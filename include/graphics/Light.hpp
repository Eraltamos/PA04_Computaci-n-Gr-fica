#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Common.hpp"

class Light {
public:
    Vector3 position;
    float ambient[4];
    float diffuse[4];
    float specular[4];

    Light();
    void apply(GLenum lightIndex = GL_LIGHT0) const;
    void moveX(float delta);
    void moveY(float delta);
    void moveZ(float delta);
};

#endif // LIGHT_HPP