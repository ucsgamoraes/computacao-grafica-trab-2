#ifndef INTERACAO_H
#define INTERACAO_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Peca;

struct Ray {
    glm::vec3 origin, direction;
};

bool cursorRay(double x, double y, int windowWidth, int windowHeight,
               int framebufferWidth, int framebufferHeight,
               const glm::mat4& view, const glm::mat4& projection, Ray& ray);
bool intersectBox(const Ray& ray, glm::vec3 minimum, glm::vec3 maximum, float& distance);
bool destinoNoTabuleiro(const Ray& ray, int& linha, int& coluna);
int selecionarPeca(const Ray& ray, const std::vector<std::unique_ptr<Peca>>& pecas);

#endif
