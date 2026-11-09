#ifndef MESH_H
#define MESH_H

#include "Object.h"
#include <vector>

struct MeshData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

// Posição (3), cor (3) e UV (2), como GLFW_Textures.
class Mesh : public Object {
public:
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    explicit Mesh(const MeshData& data) : Mesh(data.vertices, data.indices) {}
    ~Mesh() override;
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    void draw(Shader& shader, glm::mat4 parentTransform) const override;
    bool intersectRay(glm::vec3 origin, glm::vec3 direction, glm::mat4 parentTransform, float& distance) const;

    // true: desenhada com o shader de textura; false: com o shader de cor.
    bool texturizada = true;

private:
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    GLsizei indexCount;
    glm::vec3 minimum, maximum;
};

#endif
