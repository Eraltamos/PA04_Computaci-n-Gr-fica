#ifndef MODEL_OBJ_HPP
#define MODEL_OBJ_HPP

#include "Common.hpp"
#include <string>
#include <vector>

class ModelOBJ {
private:
    GLuint displayListId;
    bool isLoaded;
    AABB localBounds;

public:
    ModelOBJ();
    ~ModelOBJ();
    bool load(const std::string& filepath);
    void render() const;
    const AABB& getLocalBounds() const;
    bool isValid() const;
};

#endif // MODEL_OBJ_HPP