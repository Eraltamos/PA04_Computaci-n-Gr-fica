#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include "Common.hpp"
#include "Material.hpp"
#include "ModelOBJ.hpp"
#include <memory>

class SceneNode {
public:
    std::string name;
    PrimitiveType type;

    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    Material material;
    int textureIndex; // 0: Sin textura, 1: Tex1, 2: Tex2

    SceneNode* parent;
    std::vector<SceneNode*> children;

    // Animación
    AnimProperty animProperty;
    AnimState animState;
    float animStartVal;
    float animEndVal;
    float animDuration;
    float animCurrentTime;

    // Estado físico / colisiones
    bool isColliding;
    std::shared_ptr<ModelOBJ> meshOBJ;

    SceneNode(const std::string& nodeName, PrimitiveType nodeType);
    void render(const std::vector<GLuint>& textures);
    void renderLocalGeometry(const std::vector<GLuint>& textures);

    AABB getWorldAABB() const;
    Vector3 getWorldPosition() const;
    void updateAnimation(float dt);
};

#endif // SCENE_NODE_HPP