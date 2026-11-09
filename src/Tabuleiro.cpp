#include "Tabuleiro.h"
#include <cmath>

Tabuleiro::Tabuleiro() : base(glm::vec3(0.72f, 0.46f, 0.26f)) {  // tom da madeira da base
    base.position = glm::vec3(0.0f, -0.28f, 0.0f);
    base.scale = glm::vec3(8.5f, 0.32f, 8.5f);
    for (int linha = 0; linha < 8; ++linha) {
        for (int coluna = 0; coluna < 8; ++coluna) {
            const bool clara = (linha + coluna) % 2 != 0;
            auto quadrado = std::make_unique<Cube>(clara ? glm::vec3(0.90f, 0.85f, 0.71f)
                                                        : glm::vec3(0.23f, 0.32f, 0.31f));
            quadrado->position = centro(linha, coluna) + glm::vec3(0.0f, -0.06f, 0.0f);
            quadrado->scale = glm::vec3(1.0f, 0.12f, 1.0f);
            casas.push_back(std::move(quadrado));
        }
    }
}

void Tabuleiro::drawCasas(Shader& shader) const {
    for (const auto& quadrado : casas) quadrado->draw(shader, glm::mat4(1.0f));
}

void Tabuleiro::drawBase(Shader& shader) const {
    base.draw(shader, glm::mat4(1.0f));
}

// A coluna A fica em x = +3.5 e a H em x = -3.5. Vista do lado das brancas
// (câmera em z negativo olhando para +z), +x aparece à esquerda da tela,
// então A fica à esquerda, como num tabuleiro real.
glm::vec3 Tabuleiro::centro(int linha, int coluna) {
    return glm::vec3(3.5f - coluna, 0.0f, linha - 3.5f);
}

// Inverso de centro(): ponto do tabuleiro -> linha e coluna da casa.
bool Tabuleiro::casa(glm::vec3 ponto, int& linha, int& coluna) {
    if (!std::isfinite(ponto.x) || !std::isfinite(ponto.z) ||
        ponto.x <= -4.0f || ponto.x > 4.0f || ponto.z < -4.0f || ponto.z >= 4.0f)
        return false;
    coluna = static_cast<int>(std::floor(4.0f - ponto.x));
    linha = static_cast<int>(std::floor(ponto.z + 4.0f));
    return true;
}
