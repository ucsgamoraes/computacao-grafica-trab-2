#ifndef PRIMITIVAS_H
#define PRIMITIVAS_H

#include "Mesh.h"

class Cilindro : public Mesh {
public:
    explicit Cilindro(glm::vec3 color);
};

class Esfera : public Mesh {
public:
    explicit Esfera(glm::vec3 color);
};

class Prisma : public Mesh {
public:
    explicit Prisma(glm::vec3 color);
};

#endif
