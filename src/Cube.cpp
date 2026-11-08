// Geometria copiada de GLFW_Cubo_OO/src/Cube.cpp; cores acrescentadas por face.
#include "Cube.h"

namespace {
const float vertices[] = {
    // positions          // tex coords
    // back face
   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
    // front face
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    // left face
   -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
    // right face
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
    // top face
   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    // bottom face
   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, 1.0f
};

const std::vector<unsigned int> indices = {
    0,1,2, 2,3,0,        // back
    4,5,6, 6,7,4,        // front
    8,9,10, 10,11,8,     // left
    12,13,14, 14,15,12,  // right
    16,17,18, 18,19,16,  // top
    20,21,22, 22,23,20   // bottom
};

std::vector<float> coloredVertices(glm::vec3 color) {
    std::vector<float> result;
    const float shades[] = {0.68f, 0.92f, 0.74f, 0.86f, 1.0f, 0.55f};
    for (int i = 0; i < 24; ++i) {
        const float* vertex = vertices + i * 5;
        result.insert(result.end(), vertex, vertex + 3);
        glm::vec3 faceColor = color * shades[i / 4];
        result.insert(result.end(), {faceColor.r, faceColor.g, faceColor.b, vertex[3], vertex[4]});
    }
    return result;
}
}

Cube::Cube(glm::vec3 color) : Mesh(coloredVertices(color), indices) {}
