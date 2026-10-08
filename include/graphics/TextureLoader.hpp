#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include "Common.hpp"

class TextureLoader {
public:
    static GLuint loadBMP(const std::string& filepath);
    static GLuint createCheckerboardTexture();
};

#endif // TEXTURE_LOADER_HPP