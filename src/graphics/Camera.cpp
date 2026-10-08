#include "Camera.hpp"

Camera::Camera() 
    : yaw(45.0f), pitch(30.0f), distance(12.0f), target(0.0f, 0.0f, 0.0f),
      isPerspective(true), isWireframe(false) {}

void Camera::setupProjection(int width, int height) const {
    if (height <= 0) height = 1;
    float aspect = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (isPerspective) {
        gluPerspective(45.0, aspect, 0.1, 150.0);
    } else {
        float orthoSize = distance * 0.4f;
        glOrtho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, -100.0, 100.0);
    }

    glMatrixMode(GL_MODELVIEW);
}

void Camera::setupView() const {
    glLoadIdentity();

    float radYaw = yaw * 3.14159265f / 180.0f;
    float radPitch = pitch * 3.14159265f / 180.0f;

    float eyeX = target.x + distance * std::cos(radPitch) * std::sin(radYaw);
    float eyeY = target.y + distance * std::sin(radPitch);
    float eyeZ = target.z + distance * std::cos(radPitch) * std::cos(radYaw);

    gluLookAt(eyeX, eyeY, eyeZ, target.x, target.y, target.z, 0.0, 1.0, 0.0);
}

void Camera::orbit(float dYaw, float dPitch) {
    yaw += dYaw;
    pitch += dPitch;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

void Camera::zoom(float delta) {
    distance += delta;
    if (distance < 2.0f) distance = 2.0f;
    if (distance > 60.0f) distance = 60.0f;
}

void Camera::toggleProjection() {
    isPerspective = !isPerspective;
}

void Camera::togglePolygonMode() {
    isWireframe = !isWireframe;
    if (isWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Camera::renderGrid(float size, int divisions) const {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glBegin(GL_LINES);
    float step = size / divisions;
    for (int i = -divisions; i <= divisions; ++i) {
        float pos = i * step;
        if (i == 0) {
            glColor3f(0.6f, 0.6f, 0.6f); // Eje central
        } else {
            glColor3f(0.25f, 0.25f, 0.25f);
        }
        glVertex3f(pos, 0.0f, -size);
        glVertex3f(pos, 0.0f, size);
        glVertex3f(-size, 0.0f, pos);
        glVertex3f(size, 0.0f, pos);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}