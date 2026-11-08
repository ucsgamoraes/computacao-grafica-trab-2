// Buffers e desenho indexado adaptados de GLFW_Cubo_OO/src/Cube.cpp.
#include "Mesh.h"
#include "Interacao.h"
#include <limits>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
    : indexCount(static_cast<GLsizei>(indices.size())) {
    minimum = glm::vec3(std::numeric_limits<float>::infinity());
    maximum = -minimum;
    for (std::size_t i = 0; i < vertices.size(); i += 8) {
        const glm::vec3 point(vertices[i], vertices[i + 1], vertices[i + 2]);
        minimum = glm::min(minimum, point);
        maximum = glm::max(maximum, point);
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw(Shader& shader, glm::mat4 parentTransform) const {
    shader.setMat4("model", modelMatrix(parentTransform));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

bool Mesh::intersectRay(glm::vec3 origin, glm::vec3 direction, glm::mat4 parentTransform, float& distance) const {
    const glm::mat4 inverse = glm::inverse(modelMatrix(parentTransform));
    const Ray local{glm::vec3(inverse * glm::vec4(origin, 1)),
                    glm::vec3(inverse * glm::vec4(direction, 0))};
    // Não normalizar a direção local: o parâmetro permanece na escala do raio mundial.
    return intersectBox(local, minimum, maximum, distance);
}
