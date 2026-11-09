#ifndef OBJECT_H
#define OBJECT_H

#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

// Mesmas transformações e desenho com matriz pai usados em GLFW_Cubo_OO.
class Object {
public:
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f, 1.0f, 0.0f};
    glm::vec3 scale{1.0f};
    float angle = 0.0f;

    virtual ~Object() = default;
    // Vazio por padrão, como no exemplo do professor. Mesh desenha a malha;
    // Peca usa desenhar(), que recebe os dois shaders.
    virtual void draw(Shader&, glm::mat4) const {}
    glm::mat4 modelMatrix(glm::mat4 parentTransform = glm::mat4(1.0f)) const;
};

#endif
