#ifndef ANIMATION_CONTROLLER_HPP
#define ANIMATION_CONTROLLER_HPP

#include "Common.hpp"

class Scene;

class AnimationController {
public:
    static void togglePlayPause(Scene& scene);
    static void stopAndReset(Scene& scene);
    static void cycleAnimProperty(Scene& scene);
};

#endif // ANIMATION_CONTROLLER_HPP