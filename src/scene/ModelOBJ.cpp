#include "ModelOBJ.hpp"
#include <fstream>
#include <sstream>

ModelOBJ::ModelOBJ() : displayListId(0), isLoaded(false) {}

ModelOBJ::~ModelOBJ() {
    if (displayListId != 0) {
        glDeleteLists(displayListId, 1);
    }
}

bool ModelOBJ::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "[ModelOBJ] No se pudo cargar el archivo OBJ: " << filepath << "\n";
        return false;
    }

    std::vector<Vector3> temp_vertices;
    std::vector<Vector3> temp_normals;
    std::vector<int> vIndices, nIndices;

    Vector3 minB(1e6f, 1e6f, 1e6f);
    Vector3 maxB(-1e6f, -1e6f, -1e6f);

    std::string line;
    while (std::getline(file, line)) {
        if (line.substr(0, 2) == "v ") {
            std::istringstream s(line.substr(2));
            Vector3 v;
            s >> v.x >> v.y >> v.z;
            temp_vertices.push_back(v);

            if (v.x < minB.x) minB.x = v.x;
            if (v.y < minB.y) minB.y = v.y;
            if (v.z < minB.z) minB.z = v.z;
            if (v.x > maxB.x) maxB.x = v.x;
            if (v.y > maxB.y) maxB.y = v.y;
            if (v.z > maxB.z) maxB.z = v.z;
        } else if (line.substr(0, 3) == "vn ") {
            std::istringstream s(line.substr(3));
            Vector3 n;
            s >> n.x >> n.y >> n.z;
            temp_normals.push_back(n);
        } else if (line.substr(0, 2) == "f ") {
            std::istringstream s(line.substr(2));
            std::string part;
            while (s >> part) {
                // Formato v/vt/vn o v//vn o v
                int vi = 0, ti = 0, ni = 0;
                size_t slash1 = part.find('/');
                if (slash1 == std::string::npos) {
                    vi = std::stoi(part);
                } else {
                    vi = std::stoi(part.substr(0, slash1));
                    size_t slash2 = part.find('/', slash1 + 1);
                    if (slash2 != std::string::npos && slash2 + 1 < part.length()) {
                        ni = std::stoi(part.substr(slash2 + 1));
                    }
                }
                vIndices.push_back(vi - 1);
                nIndices.push_back(ni > 0 ? ni - 1 : -1);
            }
        }
    }

    localBounds = AABB(minB, maxB);

    displayListId = glGenLists(1);
    glNewList(displayListId, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < vIndices.size(); ++i) {
        int vi = vIndices[i];
        int ni = nIndices[i];
        if (ni >= 0 && ni < (int)temp_normals.size()) {
            glNormal3f(temp_normals[ni].x, temp_normals[ni].y, temp_normals[ni].z);
        }
        if (vi >= 0 && vi < (int)temp_vertices.size()) {
            glVertex3f(temp_vertices[vi].x, temp_vertices[vi].y, temp_vertices[vi].z);
        }
    }
    glEnd();
    glEndList();

    isLoaded = true;
    return true;
}

void ModelOBJ::render() const {
    if (isLoaded && displayListId != 0) {
        glCallList(displayListId);
    }
}

const AABB& ModelOBJ::getLocalBounds() const {
    return localBounds;
}

bool ModelOBJ::isValid() const {
    return isLoaded;
}