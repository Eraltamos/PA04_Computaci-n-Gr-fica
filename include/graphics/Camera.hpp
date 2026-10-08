#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Common.hpp"

class Camera {
public:
    float yaw;
    float pitch;
    float distance;
    Vector3 target;
    bool isPerspective;
    bool isWireframe;

    Camera();
    void setupProjection(int width, int height) const;
    void setupView() const;
    void orbit(float dYaw, float dPitch);
    void zoom(float delta);
    void toggleProjection();
    void togglePolygonMode();
    void renderGrid(float size, int divisions) const;
};

#endif // CAMERA_HPP