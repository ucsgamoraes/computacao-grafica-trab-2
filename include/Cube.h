#ifndef CUBE_H
#define CUBE_H

#include "Mesh.h"

class Cube : public Mesh {
public:
    explicit Cube(glm::vec3 color = glm::vec3(1.0f));
};

#endif
