#ifndef SCENE_HPP
#define SCENE_HPP

#include "Common.hpp"
#include "SceneNode.hpp"
#include "ModelOBJ.hpp"
#include <vector>
#include <memory>
#include <string>

class Scene {
private:
    std::vector<std::unique_ptr<SceneNode>> nodes;
    int selectedIndex;
    std::shared_ptr<ModelOBJ> sharedOBJ;
    int nodeCounter;

public:
    Scene();
    void addNode(PrimitiveType type, const std::string& customName = "");
    void importOBJ(const std::string& filepath);
    void duplicateSelected();
    void deleteSelected();

    void selectNext();
    void selectPrev();
    SceneNode* getSelectedNode() const;
    int getSelectedIndex() const;

    void setParent(SceneNode* child, SceneNode* parent);
    void detachParent(SceneNode* child);

    void checkCollisions();
    void update(float dt);
    void render(const std::vector<GLuint>& textures);

    bool saveToFile(const std::string& filepath) const;
    bool loadFromFile(const std::string& filepath);

    const std::vector<std::unique_ptr<SceneNode>>& getNodes() const;
};

#endif // SCENE_HPP