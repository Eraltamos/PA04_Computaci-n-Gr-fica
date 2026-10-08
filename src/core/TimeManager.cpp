#include "TimeManager.hpp"

TimeManager::TimeManager() : lastTimeMs(0), deltaTime(0.0f), totalTime(0.0f) {
    lastTimeMs = glutGet(GLUT_ELAPSED_TIME);
}

void TimeManager::update() {
    int currentTimeMs = glutGet(GLUT_ELAPSED_TIME);
    int diffMs = currentTimeMs - lastTimeMs;
    lastTimeMs = currentTimeMs;

    deltaTime = diffMs / 1000.0f;
    if (deltaTime > 0.1f) deltaTime = 0.1f; // Limitar picos por pausas
    totalTime += deltaTime;
}

float TimeManager::getDeltaTime() const {
    return deltaTime;
}

float TimeManager::getTotalTime() const {
    return totalTime;
}