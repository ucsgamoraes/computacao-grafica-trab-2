#include "Interacao.h"
#include "Tabuleiro.h"
#include "Peca.h"
#include <algorithm>
#include <cmath>
#include <limits>

bool cursorRay(double x, double y, int windowWidth, int windowHeight,
               int framebufferWidth, int framebufferHeight,
               const glm::mat4& view, const glm::mat4& projection, Ray& ray) {
    if (windowWidth <= 0 || windowHeight <= 0 || framebufferWidth <= 0 || framebufferHeight <= 0 ||
        !std::isfinite(x) || !std::isfinite(y) || x < 0 || y < 0 || x >= windowWidth || y >= windowHeight)
        return false;
    const double pixelX = x * framebufferWidth / windowWidth;
    const double pixelY = y * framebufferHeight / windowHeight;
    const float ndcX = static_cast<float>(2.0 * pixelX / framebufferWidth - 1.0);
    const float ndcY = static_cast<float>(1.0 - 2.0 * pixelY / framebufferHeight);
    const glm::mat4 inverse = glm::inverse(projection * view);
    glm::vec4 nearPoint = inverse * glm::vec4(ndcX, ndcY, -1, 1);
    glm::vec4 farPoint = inverse * glm::vec4(ndcX, ndcY, 1, 1);
    if (std::abs(nearPoint.w) < 1e-6f || std::abs(farPoint.w) < 1e-6f) return false;
    nearPoint /= nearPoint.w;
    farPoint /= farPoint.w;
    const glm::vec3 delta = glm::vec3(farPoint - nearPoint);
    if (!std::isfinite(glm::length(delta)) || glm::length(delta) < 1e-6f) return false;
    ray = {glm::vec3(nearPoint), glm::normalize(delta)};
    return true;
}

// Método dos intervalos por eixo; aceita também a origem dentro do volume.
bool intersectBox(const Ray& ray, glm::vec3 minimum, glm::vec3 maximum, float& distance) {
    float entry = 0.0f, exit = std::numeric_limits<float>::infinity();
    for (int axis = 0; axis < 3; ++axis) {
        if (std::abs(ray.direction[axis]) < 1e-7f) {
            if (ray.origin[axis] < minimum[axis] || ray.origin[axis] > maximum[axis]) return false;
        } else {
            float a = (minimum[axis] - ray.origin[axis]) / ray.direction[axis];
            float b = (maximum[axis] - ray.origin[axis]) / ray.direction[axis];
            if (a > b) std::swap(a, b);
            entry = std::max(entry, a);
            exit = std::min(exit, b);
            if (entry > exit) return false;
        }
    }
    distance = entry;
    return exit >= 0.0f;
}

bool destinoNoTabuleiro(const Ray& ray, int& linha, int& coluna) {
    if (std::abs(ray.direction.y) < 1e-6f) return false;
    const float distance = -ray.origin.y / ray.direction.y;
    if (distance < 0.0f || !std::isfinite(distance)) return false;
    return Tabuleiro::casa(ray.origin + distance * ray.direction, linha, coluna);
}

int selecionarPeca(const Ray& ray, const std::vector<std::unique_ptr<Peca>>& pecas) {
    float nearest = std::numeric_limits<float>::infinity(), boardDistance;
    // Casas e base também ocluem as peças, inclusive quando vistas por baixo.
    if (intersectBox(ray, {-4,-0.12f,-4}, {4,0,4}, boardDistance)) nearest = boardDistance;
    if (intersectBox(ray, {-4.25f,-0.44f,-4.25f}, {4.25f,-0.12f,4.25f}, boardDistance))
        nearest = std::min(nearest, boardDistance);
    int selected = -1;
    for (std::size_t i = 0; i < pecas.size(); ++i) {
        float distance;
        if (pecas[i]->intersectRay(ray.origin, ray.direction, distance) && distance < nearest) {
            nearest = distance;
            selected = static_cast<int>(i);
        }
    }
    return selected;
}
