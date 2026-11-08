#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

class Texture {
public:
    explicit Texture(const std::string& path, bool flip = true);
    ~Texture();
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    bool isValid() const { return ID != 0; }
    void bind(unsigned int unit = 0) const;

private:
    unsigned int ID = 0;
};

#endif
