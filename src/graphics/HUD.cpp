#include "HUD.hpp"
#include "Scene.hpp"
#include "SceneNode.hpp"
#include <iomanip>
#include <sstream>

void HUD::drawString(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
}

void HUD::render(const Scene& scene, int screenWidth, int screenHeight, 
                 bool isPerspective, bool isWireframe, TransformMode mode) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, screenHeight, 0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Barra de estado superior
    glColor3f(0.9f, 0.9f, 0.2f);
    std::string topBar = "Editor 3D | Proyeccion: " + 
                         std::string(isPerspective ? "PERSPECTIVA [P]" : "ORTOGONAL [P]") +
                         " | Raster: " + std::string(isWireframe ? "ALAMBRICO [W]" : "SOLIDO [W]");
    drawString(15, 20, topBar);

    std::string modeStr = "Modo TRS: ";
    if (mode == TransformMode::TRANSLATION) modeStr += "TRASLACION [T] (I/K, O/L, U/J)";
    else if (mode == TransformMode::ROTATION) modeStr += "ROTACION [R] (I/K, O/L, U/J)";
    else modeStr += "ESCALA [E] (I/K, O/L, U/J)";
    glColor3f(0.2f, 1.0f, 0.4f);
    drawString(15, 38, modeStr);

    // Panel izquierdo arbol de escena
    glColor3f(0.4f, 0.8f, 1.0f);
    drawString(15, 70, "   ARBOL DE ESCENA (TAB: Seleccionar)    ");

    int yOffset = 90;
    const auto& nodes = scene.getNodes();
    int selIdx = scene.getSelectedIndex();

    if (nodes.empty()) {
        glColor3f(0.7f, 0.7f, 0.7f);
        drawString(20, yOffset, "[Escena Vacia - Pulse 1-5 para crear]");
    } else {
        for (int i = 0; i < (int)nodes.size(); ++i) {
            std::string prefix = (i == selIdx) ? "-> " : "   ";
            if (nodes[i]->parent != nullptr) prefix += "  [Hijo] ";
            else prefix += "[Raiz] ";

            std::string line = prefix + std::to_string(i) + ": " + nodes[i]->name;
            if (i == selIdx) glColor3f(1.0f, 1.0f, 0.0f);
            else glColor3f(0.8f, 0.8f, 0.8f);

            drawString(15, yOffset, line);
            yOffset += 18;
            if (yOffset > screenHeight - 160) break;
        }
    }

    // Panel derecho inspector de propiedades
    int rightCol = screenWidth - 360;
    if (rightCol > 250) {
        glColor3f(0.4f, 0.8f, 1.0f);
        drawString(rightCol, 70, " PROPIEDADES OBJETO ACTIVO");

        SceneNode* selNode = scene.getSelectedNode();
        if (selNode) {
            glColor3f(1.0f, 1.0f, 1.0f);
            drawString(rightCol, 90, "Nombre: " + selNode->name);
            drawString(rightCol, 108, "Padre: " + (selNode->parent ? selNode->parent->name : "NINGUNO"));

            std::ostringstream ssPos, ssRot, ssScl;
            ssPos << std::fixed << std::setprecision(2) << "Pos: [" << selNode->position.x 
                  << ", " << selNode->position.y << ", " << selNode->position.z << "]";
            ssRot << std::fixed << std::setprecision(2) << "Rot: [" << selNode->rotation.x 
                  << ", " << selNode->rotation.y << ", " << selNode->rotation.z << "]";
            ssScl << std::fixed << std::setprecision(2) << "Scl: [" << selNode->scale.x 
                  << ", " << selNode->scale.y << ", " << selNode->scale.z << "]";

            drawString(rightCol, 130, ssPos.str());
            drawString(rightCol, 148, ssRot.str());
            drawString(rightCol, 166, ssScl.str());

            drawString(rightCol, 190, "Textura ID: " + std::to_string(selNode->textureIndex) + " [M: Conmutar]");
            drawString(rightCol, 208, "Brillo Shininess: " + std::to_string((int)selNode->material.shininess) + " [B]");

            // Estado de animación
            std::string animStr = "Animacion: ";
            if (selNode->animState == AnimState::PLAYING) animStr += "PLAY (Espacio)";
            else if (selNode->animState == AnimState::PAUSED) animStr += "PAUSA (Espacio)";
            else animStr += "STOP (Z: Reset)";
            drawString(rightCol, 235, animStr);

            // Alerta de colisión
            if (selNode->isColliding) {
                glColor3f(1.0f, 0.2f, 0.2f);
                drawString(rightCol, 260, "[ ! ALERTA: COLISION AABB ! ]");
            } else {
                glColor3f(0.2f, 1.0f, 0.2f);
                drawString(rightCol, 260, "[Espacio Libre: Sin Colision]");
            }
        } else {
            glColor3f(0.6f, 0.6f, 0.6f);
            drawString(rightCol, 90, "Ningun objeto seleccionado.");
        }
    }

    // Comandos en la parte inferior
    glColor3f(0.8f, 0.8f, 0.8f);
    int bottomY = screenHeight - 40;
    drawString(15, bottomY, "Crear: 1:Cubo 2:Esfera 3:Cilindro 4:Plano 5:OBJ | D:Duplicar | X:Borrar | H:Emparentar N:Desvincular");
    drawString(15, bottomY + 18, "F5:Guardar Escena | F6:Cargar Escena | G:Mover Luz | Flechas:Orbitar | +/-:Zoom");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}