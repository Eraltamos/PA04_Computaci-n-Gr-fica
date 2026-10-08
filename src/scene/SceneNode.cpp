#include "SceneNode.hpp"
#include "Geometry.hpp"

SceneNode::SceneNode(const std::string& nodeName, PrimitiveType nodeType)
    : name(nodeName), type(nodeType),
      position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f),
      textureIndex(0), parent(nullptr),
      animProperty(AnimProperty::ROT_Y), animState(AnimState::STOPPED),
      animStartVal(0.0f), animEndVal(360.0f), animDuration(4.0f), animCurrentTime(0.0f),
      isColliding(false) {}

void SceneNode::render(const std::vector<GLuint>& textures) {
    glPushMatrix();

    // Transformación local concatenada en la pila matricial
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(scale.x, scale.y, scale.z);

    renderLocalGeometry(textures);

    // Propagación jerárquica a descendientes
    for (SceneNode* child : children) {
        if (child) child->render(textures);
    }

    glPopMatrix();
}

void SceneNode::renderLocalGeometry(const std::vector<GLuint>& textures) {
    material.apply();

    if (textureIndex > 0 && textureIndex <= (int)textures.size()) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[textureIndex - 1]);
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    switch (type) {
        case PrimitiveType::CUBE:     Geometry::drawCube(); break;
        case PrimitiveType::SPHERE:   Geometry::drawSphere(); break;
        case PrimitiveType::CYLINDER: Geometry::drawCylinder(); break;
        case PrimitiveType::PLANE:    Geometry::drawPlane(); break;
        case PrimitiveType::OBJ_MESH:
            if (meshOBJ && meshOBJ->isValid()) meshOBJ->render();
            else Geometry::drawCube();
            break;
    }

    glDisable(GL_TEXTURE_2D);
}

Vector3 SceneNode::getWorldPosition() const {
    if (parent == nullptr) return position;
    return parent->getWorldPosition() + position; // Aproximación traslacional para AABB
}

AABB SceneNode::getWorldAABB() const {
    Vector3 worldPos = getWorldPosition();
    Vector3 halfExtents(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f);

    if (type == PrimitiveType::OBJ_MESH && meshOBJ && meshOBJ->isValid()) {
        AABB localB = meshOBJ->getLocalBounds();
        halfExtents.x = (localB.max.x - localB.min.x) * 0.5f * scale.x;
        halfExtents.y = (localB.max.y - localB.min.y) * 0.5f * scale.y;
        halfExtents.z = (localB.max.z - localB.min.z) * 0.5f * scale.z;
    }

    return AABB(worldPos - halfExtents, worldPos + halfExtents);
}

void SceneNode::updateAnimation(float dt) {
    if (animState != AnimState::PLAYING || animDuration <= 0.0f) return;

    animCurrentTime += dt;
    if (animCurrentTime > animDuration) animCurrentTime = 0.0f; // Ciclo continuo

    float alpha = animCurrentTime / animDuration;
    float currentVal = animStartVal + alpha * (animEndVal - animStartVal); // LERP

    switch (animProperty) {
        case AnimProperty::POS_X:   position.x = currentVal; break;
        case AnimProperty::POS_Y:   position.y = currentVal; break;
        case AnimProperty::POS_Z:   position.z = currentVal; break;
        case AnimProperty::ROT_X:   rotation.x = currentVal; break;
        case AnimProperty::ROT_Y:   rotation.y = currentVal; break;
        case AnimProperty::ROT_Z:   rotation.z = currentVal; break;
        case AnimProperty::SCALE_X: scale.x = currentVal; break;
        case AnimProperty::SCALE_Y: scale.y = currentVal; break;
        case AnimProperty::SCALE_Z: scale.z = currentVal; break;
        case AnimProperty::NONE: break;
    }
}