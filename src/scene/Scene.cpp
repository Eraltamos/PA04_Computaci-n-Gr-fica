#include "Scene.hpp"
#include "Geometry.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

Scene::Scene() : selectedIndex(-1), nodeCounter(1) {
}

void Scene::addNode(PrimitiveType type, const std::string& customName) {
    std::string baseName = customName;
    if (baseName.empty()) {
        switch (type) {
            case PrimitiveType::CUBE:     baseName = "Cubo_" + std::to_string(nodeCounter++); break;
            case PrimitiveType::SPHERE:   baseName = "Esfera_" + std::to_string(nodeCounter++); break;
            case PrimitiveType::CYLINDER: baseName = "Cilindro_" + std::to_string(nodeCounter++); break;
            case PrimitiveType::PLANE:    baseName = "Plano_" + std::to_string(nodeCounter++); break;
            case PrimitiveType::OBJ_MESH: baseName = "MallaOBJ_" + std::to_string(nodeCounter++); break;
        }
    }

    auto newNode = std::make_unique<SceneNode>(baseName, type);
    newNode->position = Vector3(0.0f, 0.5f, 0.0f);

    if (type == PrimitiveType::OBJ_MESH && sharedOBJ) {
        newNode->meshOBJ = sharedOBJ;
    }

    nodes.push_back(std::move(newNode));
    selectedIndex = (int)nodes.size() - 1;
}

void Scene::importOBJ(const std::string& filepath) {
    sharedOBJ = std::make_shared<ModelOBJ>();
    if (sharedOBJ->load(filepath)) {
        addNode(PrimitiveType::OBJ_MESH, "Pieza_OBJ");
    } else {
        std::cerr << "[Scene] Fallo al cargar OBJ. Creando cubo sustituto.\n";
        addNode(PrimitiveType::CUBE, "Cubo_Respaldo");
    }
}

void Scene::duplicateSelected() {
    SceneNode* src = getSelectedNode();
    if (!src) return;

    auto clone = std::make_unique<SceneNode>(src->name + "_Copia", src->type);
    clone->position = src->position + Vector3(1.0f, 0.0f, 0.0f);
    clone->rotation = src->rotation;
    clone->scale = src->scale;
    clone->material = src->material;
    clone->textureIndex = src->textureIndex;
    clone->meshOBJ = src->meshOBJ;

    nodes.push_back(std::move(clone));
    selectedIndex = (int)nodes.size() - 1;
}

void Scene::deleteSelected() {
    if (selectedIndex < 0 || selectedIndex >= (int)nodes.size()) return;

    SceneNode* toDelete = nodes[selectedIndex].get();
    detachParent(toDelete);

    // Liberar y desvincular hijos
    for (SceneNode* child : toDelete->children) {
        if (child) child->parent = nullptr;
    }

    nodes.erase(nodes.begin() + selectedIndex);
    if (nodes.empty()) {
        selectedIndex = -1;
    } else if (selectedIndex >= (int)nodes.size()) {
        selectedIndex = (int)nodes.size() - 1;
    }
}

void Scene::selectNext() {
    if (nodes.empty()) return;
    selectedIndex = (selectedIndex + 1) % nodes.size();
}

void Scene::selectPrev() {
    if (nodes.empty()) return;
    selectedIndex = (selectedIndex - 1 + (int)nodes.size()) % nodes.size();
}

SceneNode* Scene::getSelectedNode() const {
    if (selectedIndex >= 0 && selectedIndex < (int)nodes.size()) {
        return nodes[selectedIndex].get();
    }
    return nullptr;
}

int Scene::getSelectedIndex() const {
    return selectedIndex;
}

void Scene::setParent(SceneNode* child, SceneNode* parent) {
    if (!child || !parent || child == parent) return;

    // Prevencion de ciclos: verificar que 'parent' no sea ya un descendiente de 'child'
    SceneNode* ancestor = parent;
    while (ancestor != nullptr) {
        if (ancestor == child) {
            std::cerr << "[Scene] Advertencia: Operacion cancelada para evitar ciclo jerarquico.\n";
            return;
        }
        ancestor = ancestor->parent;
    }

    detachParent(child);
    child->parent = parent;
    parent->children.push_back(child);
}

void Scene::detachParent(SceneNode* child) {
    if (!child || !child->parent) return;
    auto& sibs = child->parent->children;
    sibs.erase(std::remove(sibs.begin(), sibs.end(), child), sibs.end());
    child->parent = nullptr;
}

void Scene::checkCollisions() {
    for (auto& n : nodes) n->isColliding = false;

    SceneNode* active = getSelectedNode();
    if (!active) return;

    AABB activeBox = active->getWorldAABB();
    for (auto& n : nodes) {
        if (n.get() == active) continue;
        AABB otherBox = n->getWorldAABB();
        if (activeBox.intersects(otherBox)) {
            active->isColliding = true;
            n->isColliding = true;
        }
    }
}

void Scene::update(float dt) {
    for (auto& n : nodes) {
        n->updateAnimation(dt);
    }
    checkCollisions();
}

void Scene::render(const std::vector<GLuint>& textures) {
    // Renderizado jerárquico solo desde raíces
    for (auto& n : nodes) {
        if (n->parent == nullptr) {
            n->render(textures);
        }
    }

    // Dibujo del AABB de colisión en modo Wireframe
    SceneNode* active = getSelectedNode();
    if (active) {
        AABB box = active->getWorldAABB();
        if (active->isColliding) glColor3f(1.0f, 0.0f, 0.0f); // Rojo: colisión
        else glColor3f(0.0f, 1.0f, 0.0f);                     // Verde: libre
        Geometry::drawWireAABB(box);
    }
}

bool Scene::saveToFile(const std::string& filepath) const {
    std::ofstream out(filepath);
    if (!out) return false;

    out << "SCENE_FILE_V1\n";
    out << nodes.size() << "\n";

    for (const auto& n : nodes) {
        out << n->name << " " << (int)n->type << " ";
        out << (n->parent ? n->parent->name : "NONE") << " ";
        out << n->position.x << " " << n->position.y << " " << n->position.z << " ";
        out << n->rotation.x << " " << n->rotation.y << " " << n->rotation.z << " ";
        out << n->scale.x << " " << n->scale.y << " " << n->scale.z << " ";
        out << n->material.shininess << " " << n->textureIndex << "\n";
    }
    return true;
}

bool Scene::loadFromFile(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in) return false;

    std::string header;
    in >> header;
    if (header != "SCENE_FILE_V1") return false;

    size_t count = 0;
    in >> count;

    nodes.clear();
    selectedIndex = -1;

    struct SerializedRecord {
        std::string name;
        int type;
        std::string parentName;
        Vector3 pos, rot, scl;
        float shininess;
        int texIdx;
    };

    std::vector<SerializedRecord> records;
    for (size_t i = 0; i < count; ++i) {
        SerializedRecord r;
        in >> r.name >> r.type >> r.parentName
           >> r.pos.x >> r.pos.y >> r.pos.z
           >> r.rot.x >> r.rot.y >> r.rot.z
           >> r.scl.x >> r.scl.y >> r.scl.z
           >> r.shininess >> r.texIdx;
        records.push_back(r);
    }

    // Instanciación
    for (const auto& r : records) {
        auto node = std::make_unique<SceneNode>(r.name, (PrimitiveType)r.type);
        node->position = r.pos;
        node->rotation = r.rot;
        node->scale = r.scl;
        node->material.shininess = r.shininess;
        node->textureIndex = r.texIdx;
        if (node->type == PrimitiveType::OBJ_MESH && sharedOBJ) {
            node->meshOBJ = sharedOBJ;
        }
        nodes.push_back(std::move(node));
    }

    // Reconstrucción de jerarquías
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].parentName != "NONE") {
            for (size_t j = 0; j < nodes.size(); ++j) {
                if (nodes[j]->name == records[i].parentName) {
                    setParent(nodes[i].get(), nodes[j].get());
                    break;
                }
            }
        }
    }

    if (!nodes.empty()) selectedIndex = 0;
    return true;
}

const std::vector<std::unique_ptr<SceneNode>>& Scene::getNodes() const {
    return nodes;
}