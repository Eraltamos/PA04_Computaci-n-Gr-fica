#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "Common.hpp"

class Geometry {
public:
    static void drawCube();
    static void drawSphere(float radius = 0.5f, int slices = 16, int stacks = 16);
    static void drawCylinder(float radius = 0.5f, float height = 1.0f, int slices = 16);
    static void drawPlane(float size = 2.0f);
    static void drawWireAABB(const AABB& box);
};

#endif // GEOMETRY_HPP