#include "Light.hpp"

Light::Light() : position(3.0f, 6.0f, 4.0f) {
    ambient[0] = 0.2f; ambient[1] = 0.2f; ambient[2] = 0.2f; ambient[3] = 1.0f;
    diffuse[0] = 0.9f; diffuse[1] = 0.9f; diffuse[2] = 0.9f; diffuse[3] = 1.0f;
    specular[0] = 1.0f; specular[1] = 1.0f; specular[2] = 1.0f; specular[3] = 1.0f;
}

void Light::apply(GLenum lightIndex) const {
    float lightPos[4] = { position.x, position.y, position.z, 1.0f }; // Luz puntual
    glLightfv(lightIndex, GL_POSITION, lightPos);
    glLightfv(lightIndex, GL_AMBIENT, ambient);
    glLightfv(lightIndex, GL_DIFFUSE, diffuse);
    glLightfv(lightIndex, GL_SPECULAR, specular);
    glEnable(lightIndex);
}

void Light::moveX(float delta) { position.x += delta; }
void Light::moveY(float delta) { position.y += delta; }
void Light::moveZ(float delta) { position.z += delta; }