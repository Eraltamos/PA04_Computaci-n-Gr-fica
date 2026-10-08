#include "TextureLoader.hpp"
#include <fstream>
#include <vector>

GLuint TextureLoader::loadBMP(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        std::cerr << "[TextureLoader] Advertencia: No se pudo abrir " << filepath 
                  << ". Usando textura de respaldo procedural.\n";
        return createCheckerboardTexture();
    }

    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), 54);
    if (file.gcount() < 54 || header[0] != 'B' || header[1] != 'M') {
        std::cerr << "[TextureLoader] Error: Encabezado BMP invalido en " << filepath << "\n";
        return createCheckerboardTexture();
    }

    int dataPos   = *(int*)&(header[0x0A]);
    int imageSize = *(int*)&(header[0x22]);
    int width     = *(int*)&(header[0x12]);
    int height    = *(int*)&(header[0x16]);

    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0) dataPos = 54;

    file.seekg(dataPos);
    std::vector<unsigned char> data(imageSize);
    file.read(reinterpret_cast<char*>(data.data()), imageSize);
    file.close();

    // Convertir BGR a RGB
    for (int i = 0; i < imageSize; i += 3) {
        unsigned char temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

    return textureID;
}

GLuint TextureLoader::createCheckerboardTexture() {
    const int size = 64;
    unsigned char img[size][size][3];
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            unsigned char color = (((r / 8) + (c / 8)) % 2 == 0) ? 220 : 60;
            img[r][c][0] = color;
            img[r][c][1] = color;
            img[r][c][2] = color;
        }
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

    return textureID;
}