#ifndef HUD_HPP
#define HUD_HPP

#include "Common.hpp"
#include <string>

class Scene;

class HUD {
private:
    static void drawString(float x, float y, const std::string& text);

public:
    static void render(const Scene& scene, int screenWidth, int screenHeight, 
                       bool isPerspective, bool isWireframe, TransformMode mode);
};

#endif // HUD_HPP