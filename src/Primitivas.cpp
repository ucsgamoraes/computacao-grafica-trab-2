// Triangulação simples; buffers e transformações seguem o cubo do professor.
#include "Primitivas.h"
#include <glm/gtc/constants.hpp>
#include <cmath>

namespace {
void vertex(MeshData& data, glm::vec3 position, glm::vec3 color, float u, float v) {
    data.vertices.insert(data.vertices.end(), {position.x, position.y, position.z,
                                             color.r, color.g, color.b, u, v});
}

MeshData cilindro(glm::vec3 color) {
    MeshData data;
    const int segments = 24;
    for (int i = 0; i <= segments; ++i) {
        const float u = static_cast<float>(i) / segments;
        const float angle = glm::two_pi<float>() * u;
        const float x = 0.5f * std::cos(angle), z = 0.5f * std::sin(angle);
        const glm::vec3 shade = color * (0.76f + 0.20f * std::cos(angle - 0.6f));
        vertex(data, glm::vec3(x, -0.5f, z), shade, u, 0);
        vertex(data, glm::vec3(x, 0.5f, z), shade, u, 1);
        if (i < segments) {
            unsigned int a = static_cast<unsigned int>(2 * i);
            data.indices.insert(data.indices.end(), {a, a + 1, a + 2, a + 1, a + 3, a + 2});
        }
    }
    for (int cap = 0; cap < 2; ++cap) {
        const float y = cap == 0 ? -0.5f : 0.5f;
        const glm::vec3 shade = color * (cap == 0 ? 0.55f : 1.0f);
        const unsigned int center = static_cast<unsigned int>(data.vertices.size() / 8);
        vertex(data, glm::vec3(0, y, 0), shade, 0.5f, 0.5f);
        for (int i = 0; i <= segments; ++i) {
            const float angle = glm::two_pi<float>() * i / segments;
            const float x = 0.5f * std::cos(angle), z = 0.5f * std::sin(angle);
            vertex(data, glm::vec3(x, y, z), shade, x + 0.5f, z + 0.5f);
            if (i < segments) {
                const unsigned int a = center + 1 + static_cast<unsigned int>(i);
                if (cap == 0) data.indices.insert(data.indices.end(), {center, a, a + 1});
                else data.indices.insert(data.indices.end(), {center, a + 1, a});
            }
        }
    }
    return data;
}

MeshData esfera(glm::vec3 color) {
    MeshData data;
    const int rings = 12, segments = 24;
    for (int r = 0; r <= rings; ++r) {
        const float v = static_cast<float>(r) / rings;
        const float latitude = glm::pi<float>() * v;
        for (int s = 0; s <= segments; ++s) {
            const float u = static_cast<float>(s) / segments;
            const float longitude = glm::two_pi<float>() * u;
            const glm::vec3 normal(std::sin(latitude) * std::cos(longitude),
                                   std::cos(latitude), std::sin(latitude) * std::sin(longitude));
            const float shade = 0.75f + 0.22f * glm::dot(normal, glm::normalize(glm::vec3(1, 2, 1)));
            vertex(data, normal * 0.5f, color * shade, u, v);
            if (r < rings && s < segments) {
                const unsigned int a = static_cast<unsigned int>(r * (segments + 1) + s);
                const unsigned int b = a + segments + 1;
                data.indices.insert(data.indices.end(), {a, b, a + 1, a + 1, b, b + 1});
            }
        }
    }
    return data;
}

MeshData prisma(glm::vec3 color) {
    MeshData data;
    const glm::vec3 points[] = {{-0.5f,-0.5f,-0.5f}, {0.5f,-0.5f,-0.5f}, {0,0.5f,-0.5f},
                                {-0.5f,-0.5f,0.5f}, {0.5f,-0.5f,0.5f}, {0,0.5f,0.5f}};
    const unsigned int faces[][3] = {{0,2,1}, {3,4,5}, {0,1,4}, {0,4,3},
                                     {1,2,5}, {1,5,4}, {2,0,3}, {2,3,5}};
    for (int face = 0; face < 8; ++face) {
        const float shade = face < 2 ? 0.85f : (face < 4 ? 0.55f : (face < 6 ? 0.95f : 0.70f));
        for (int corner = 0; corner < 3; ++corner) {
            const glm::vec3 p = points[faces[face][corner]];
            data.indices.push_back(static_cast<unsigned int>(data.vertices.size() / 8));
            vertex(data, p, color * shade, p.x + 0.5f, p.y + 0.5f);
        }
    }
    return data;
}
}

Cilindro::Cilindro(glm::vec3 color) : Mesh(cilindro(color)) {}
Esfera::Esfera(glm::vec3 color) : Mesh(esfera(color)) {}
Prisma::Prisma(glm::vec3 color) : Mesh(prisma(color)) {}
