#include "Geometry.hpp"

void Geometry::drawCube() {
    float s = 0.5f;
    glBegin(GL_QUADS);
        // Frontal
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, -s,  s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( s, -s,  s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( s,  s,  s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-s,  s,  s);

        // Trasera
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, -s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s,  s, -s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( s,  s, -s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( s, -s, -s);

        // Superior
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-s,  s, -s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-s,  s,  s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( s,  s,  s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( s,  s, -s);

        // Inferior
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, -s, -s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( s, -s, -s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( s, -s,  s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s,  s);

        // Derecha
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( s, -s, -s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( s,  s, -s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( s,  s,  s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( s, -s,  s);

        // Izquierda
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, -s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, -s,  s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-s,  s,  s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-s,  s, -s);
    glEnd();
}

void Geometry::drawSphere(float radius, int slices, int stacks) {
    const float PI = 3.14159265f;
    for (int i = 0; i < stacks; ++i) {
        float phi1 = PI * (-0.5f + (float)i / stacks);
        float phi2 = PI * (-0.5f + (float)(i + 1) / stacks);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * PI * (float)j / slices;

            float x1 = std::cos(phi1) * std::cos(theta);
            float y1 = std::sin(phi1);
            float z1 = std::cos(phi1) * std::sin(theta);
            glNormal3f(x1, y1, z1);
            glTexCoord2f((float)j / slices, (float)i / stacks);
            glVertex3f(x1 * radius, y1 * radius, z1 * radius);

            float x2 = std::cos(phi2) * std::cos(theta);
            float y2 = std::sin(phi2);
            float z2 = std::cos(phi2) * std::sin(theta);
            glNormal3f(x2, y2, z2);
            glTexCoord2f((float)j / slices, (float)(i + 1) / stacks);
            glVertex3f(x2 * radius, y2 * radius, z2 * radius);
        }
        glEnd();
    }
}

void Geometry::drawCylinder(float radius, float height, int slices) {
    const float PI = 3.14159265f;
    float halfH = height * 0.5f;

    // Cuerpo lateral
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; ++i) {
        float theta = 2.0f * PI * (float)i / slices;
        float x = std::cos(theta);
        float z = std::sin(theta);

        glNormal3f(x, 0.0f, z);
        glTexCoord2f((float)i / slices, 0.0f);
        glVertex3f(x * radius, -halfH, z * radius);

        glTexCoord2f((float)i / slices, 1.0f);
        glVertex3f(x * radius, halfH, z * radius);
    }
    glEnd();

    // Tapa superior
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, halfH, 0.0f);
    for (int i = 0; i <= slices; ++i) {
        float theta = 2.0f * PI * (float)i / slices;
        glTexCoord2f(0.5f + 0.5f * std::cos(theta), 0.5f + 0.5f * std::sin(theta));
        glVertex3f(std::cos(theta) * radius, halfH, std::sin(theta) * radius);
    }
    glEnd();

    // Tapa inferior
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, -halfH, 0.0f);
    for (int i = slices; i >= 0; --i) {
        float theta = 2.0f * PI * (float)i / slices;
        glTexCoord2f(0.5f + 0.5f * std::cos(theta), 0.5f + 0.5f * std::sin(theta));
        glVertex3f(std::cos(theta) * radius, -halfH, std::sin(theta) * radius);
    }
    glEnd();
}

void Geometry::drawPlane(float size) {
    float h = size * 0.5f;
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-h, 0.0f, -h);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-h, 0.0f,  h);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( h, 0.0f,  h);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( h, 0.0f, -h);
    glEnd();
}

void Geometry::drawWireAABB(const AABB& box) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glBegin(GL_LINE_LOOP);
        glVertex3f(box.min.x, box.min.y, box.min.z);
        glVertex3f(box.max.x, box.min.y, box.min.z);
        glVertex3f(box.max.x, box.max.y, box.min.z);
        glVertex3f(box.min.x, box.max.y, box.min.z);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex3f(box.min.x, box.min.y, box.max.z);
        glVertex3f(box.max.x, box.min.y, box.max.z);
        glVertex3f(box.max.x, box.max.y, box.max.z);
        glVertex3f(box.min.x, box.max.y, box.max.z);
    glEnd();

    glBegin(GL_LINES);
        glVertex3f(box.min.x, box.min.y, box.min.z); glVertex3f(box.min.x, box.min.y, box.max.z);
        glVertex3f(box.max.x, box.min.y, box.min.z); glVertex3f(box.max.x, box.min.y, box.max.z);
        glVertex3f(box.max.x, box.max.y, box.min.z); glVertex3f(box.max.x, box.max.y, box.max.z);
        glVertex3f(box.min.x, box.max.y, box.min.z); glVertex3f(box.min.x, box.max.y, box.max.z);
    glEnd();

    glEnable(GL_LIGHTING);
}