#include "AnimationController.hpp"
#include "Scene.hpp"
#include "SceneNode.hpp"

void AnimationController::togglePlayPause(Scene& scene) {
    SceneNode* node = scene.getSelectedNode();
    if (!node) return;

    if (node->animState == AnimState::PLAYING) {
        node->animState = AnimState::PAUSED;
    } else {
        node->animState = AnimState::PLAYING;
    }
}

void AnimationController::stopAndReset(Scene& scene) {
    SceneNode* node = scene.getSelectedNode();
    if (!node) return;

    node->animState = AnimState::STOPPED;
    node->animCurrentTime = 0.0f;
    node->rotation = Vector3(0.0f, 0.0f, 0.0f);
}

void AnimationController::cycleAnimProperty(Scene& scene) {
    SceneNode* node = scene.getSelectedNode();
    if (!node) return;

    if (node->animProperty == AnimProperty::ROT_Y) {
        node->animProperty = AnimProperty::POS_Y;
        node->animStartVal = 0.5f;
        node->animEndVal = 3.0f;
    } else if (node->animProperty == AnimProperty::POS_Y) {
        node->animProperty = AnimProperty::SCALE_X;
        node->animStartVal = 1.0f;
        node->animEndVal = 2.5f;
    } else {
        node->animProperty = AnimProperty::ROT_Y;
        node->animStartVal = 0.0f;
        node->animEndVal = 360.0f;
    }
}