#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Common.hpp"

struct Material {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess;

    Material() {
        ambient[0] = 0.2f; ambient[1] = 0.2f; ambient[2] = 0.2f; ambient[3] = 1.0f;
        diffuse[0] = 0.8f; diffuse[1] = 0.8f; diffuse[2] = 0.8f; diffuse[3] = 1.0f;
        specular[0] = 1.0f; specular[1] = 1.0f; specular[2] = 1.0f; specular[3] = 1.0f;
        shininess = 16.0f;
    }

    void apply() const {
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    }
};

#endif // MATERIAL_HPP