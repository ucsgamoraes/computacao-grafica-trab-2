// Carga e filtros adaptados de GLFW_Cubo_OO/src/Texture.cpp.
#include "Texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& path, bool flip) {
    stbi_set_flip_vertically_on_load(flip);
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Erro ao carregar textura " << path << ": " << stbi_failure_reason() << '\n';
        return;
    }
    const GLenum formats[] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};
    if (channels < 1 || channels > 4) {
        stbi_image_free(data);
        return;
    }
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLint alignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, formats[channels - 1], width, height, 0,
                 formats[channels - 1], GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    stbi_image_free(data);
}

Texture::~Texture() { if (ID) glDeleteTextures(1, &ID); }

void Texture::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}
