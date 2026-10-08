#ifndef COMMON_HPP
#define COMMON_HPP

#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#ifndef GL_LIGHT_MODEL_COLOR_CONTROL
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#endif

#ifndef GL_SEPARATE_SPECULAR_COLOR
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#endif

struct Vector3 {
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    Vector3 operator+(const Vector3& o) const { return Vector3(x + o.x, y + o.y, z + o.z); }
    Vector3 operator-(const Vector3& o) const { return Vector3(x - o.x, y - o.y, z - o.z); }
    Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }

    Vector3& operator+=(const Vector3& o) {
        x += o.x; y += o.y; z += o.z;
        return *this;
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
};

struct AABB {
    Vector3 min;
    Vector3 max;

    AABB() : min(-0.5f, -0.5f, -0.5f), max(0.5f, 0.5f, 0.5f) {}
    AABB(const Vector3& _min, const Vector3& _max) : min(_min), max(_max) {}

    bool intersects(const AABB& o) const {
        return (min.x <= o.max.x && max.x >= o.min.x) &&
               (min.y <= o.max.y && max.y >= o.min.y) &&
               (min.z <= o.max.z && max.z >= o.min.z);
    }
};

enum class PrimitiveType {
    CUBE,
    SPHERE,
    CYLINDER,
    PLANE,
    OBJ_MESH
};

enum class TransformMode {
    TRANSLATION,
    ROTATION,
    SCALE
};

enum class AnimProperty {
    NONE,
    POS_X, POS_Y, POS_Z,
    ROT_X, ROT_Y, ROT_Z,
    SCALE_X, SCALE_Y, SCALE_Z
};

enum class AnimState {
    STOPPED,
    PLAYING,
    PAUSED
};

#endif // COMMON_HPP