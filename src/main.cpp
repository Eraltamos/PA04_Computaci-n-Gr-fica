#include "Common.hpp"
#include "TimeManager.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "HUD.hpp"
#include "TextureLoader.hpp"
#include "Scene.hpp"
#include "AnimationController.hpp"

static int windowWidth = 1024;
static int windowHeight = 768;

static TimeManager gTimeManager;
static Camera gCamera;
static Light gLight;
static Scene gScene;
static TransformMode gTransformMode = TransformMode::TRANSLATION;
static std::vector<GLuint> gTextures;

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glClearColor(0.12f, 0.12f, 0.14f, 1.0f);

    // Activacion de calculo exacto de vector de vista y destello sobre texturas
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    // Carga de texturas desde assets
    gTextures.push_back(TextureLoader::loadBMP("assets/textures/texture_grid.bmp"));
    gTextures.push_back(TextureLoader::loadBMP("assets/textures/texture_metal.bmp"));
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gCamera.setupProjection(windowWidth, windowHeight);
    gCamera.setupView();

    gLight.apply(GL_LIGHT0);
    gCamera.renderGrid(10.0f, 10);

    gScene.render(gTextures);

    HUD::render(gScene, windowWidth, windowHeight, 
                gCamera.isPerspective, gCamera.isWireframe, gTransformMode);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glutPostRedisplay();
}

void idle() {
    gTimeManager.update();
    gScene.update(gTimeManager.getDeltaTime());
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    SceneNode* sel = gScene.getSelectedNode();
    float step = 0.25f;

    switch (key) {
        // Creación dinámica de geometrías
        case '1': gScene.addNode(PrimitiveType::CUBE); break;
        case '2': gScene.addNode(PrimitiveType::SPHERE); break;
        case '3': gScene.addNode(PrimitiveType::CYLINDER); break;
        case '4': gScene.addNode(PrimitiveType::PLANE); break;
        case '5': gScene.importOBJ("assets/models/sample_part.obj"); break;

        // Gestión de ciclo de vida
        case 'd': case 'D': gScene.duplicateSelected(); break;
        case 'x': case 'X': gScene.deleteSelected(); break;

        // Selección
        case 9: // TAB
            gScene.selectNext();
            break;

        // Modos de transformación
        case 't': case 'T': gTransformMode = TransformMode::TRANSLATION; break;
        case 'r': case 'R': gTransformMode = TransformMode::ROTATION; break;
        case 'e': case 'E': gTransformMode = TransformMode::SCALE; break;

        // Modificación del valor TRS en ejes
        case 'i': case 'I': // X +
            if (sel) {
                if (gTransformMode == TransformMode::TRANSLATION) sel->position.x += step;
                else if (gTransformMode == TransformMode::ROTATION) sel->rotation.x += 10.0f;
                else sel->scale.x += 0.1f;
            }
            break;
        case 'k': case 'K': // X -
            if (sel) {
                if (gTransformMode == TransformMode::TRANSLATION) sel->position.x -= step;
                else if (gTransformMode == TransformMode::ROTATION) sel->rotation.x -= 10.0f;
                else if (sel->scale.x > 0.2f) sel->scale.x -= 0.1f;
            }
            break;
        case 'o': case 'O': // Y +
            if (sel) {
                if (gTransformMode == TransformMode::TRANSLATION) sel->position.y += step;
                else if (gTransformMode == TransformMode::ROTATION) sel->rotation.y += 10.0f;
                else sel->scale.y += 0.1f;
            }
            break;
        case 'l': case 'L': // Y -
            if (sel) {
                if (gTransformMode == TransformMode::TRANSLATION) sel->position.y -= step;
                else if (gTransformMode == TransformMode::ROTATION) sel->rotation.y -= 10.0f;
                else if (sel->scale.y > 0.2f) sel->scale.y -= 0.1f;
            }
            break;
        case 'u': case 'U': // Z +
            if (sel) {
                if (gTransformMode == TransformMode::TRANSLATION) sel->position.z += step;
                else if (gTransformMode == TransformMode::ROTATION) sel->rotation.z += 10.0f;
                else sel->scale.z += 0.1f;
            }
            break;
        case 'j': case 'J': // Z -
            if (sel) {
                if (gTransformMode == TransformMode::TRANSLATION) sel->position.z -= step;
                else if (gTransformMode == TransformMode::ROTATION) sel->rotation.z -= 10.0f;
                else if (sel->scale.z > 0.2f) sel->scale.z -= 0.1f;
            }
            break;

        // Jerarquía (Emparentar / Desvincular)
        case 'h': case 'H': {
            int current = gScene.getSelectedIndex();
            if (current > 0) {
                gScene.setParent(gScene.getSelectedNode(), gScene.getNodes()[current - 1].get());
            }
            break;
        }
        case 'n': case 'N':
            gScene.detachParent(sel);
            break;

        // Apariencia y materiales
        case 'm': case 'M':
            if (sel) sel->textureIndex = (sel->textureIndex + 1) % (gTextures.size() + 1);
            break;
        case 'b': case 'B':
            if (sel) {
                sel->material.shininess += 12.0f;
                if (sel->material.shininess > 64.0f) sel->material.shininess = 8.0f;
            }
            break;

        // Luz
        case 'g': case 'G':
            gLight.moveX(0.5f);
            if (gLight.position.x > 8.0f) gLight.position.x = -8.0f;
            break;

        // Cámara y rasterizador
        case 'p': case 'P': gCamera.toggleProjection(); break;
        case 'w': case 'W': gCamera.togglePolygonMode(); break;
        case '+': case '=': gCamera.zoom(-1.0f); break;
        case '-': case '_': gCamera.zoom(1.0f); break;

        // Animación
        case ' ': AnimationController::togglePlayPause(gScene); break;
        case 'z': case 'Z': AnimationController::stopAndReset(gScene); break;
        case 'c': case 'C': AnimationController::cycleAnimProperty(gScene); break;

        // Persistencia
        case 27: exit(0); break; // ESC
    }
}

void specialKeyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:  gCamera.orbit(-5.0f, 0.0f); break;
        case GLUT_KEY_RIGHT: gCamera.orbit(5.0f, 0.0f); break;
        case GLUT_KEY_UP:    gCamera.orbit(0.0f, 5.0f); break;
        case GLUT_KEY_DOWN:  gCamera.orbit(0.0f, -5.0f); break;
        case GLUT_KEY_F5:    gScene.saveToFile("scenes/saved_workspace.scene"); break;
        case GLUT_KEY_F6:    gScene.loadFromFile("scenes/saved_workspace.scene"); break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("PA4 - Editor de Escenas 3D - Erwin Alain Felix Tayro Mosqueira");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);

    glutMainLoop();
    return 0;
}