#ifndef TABULEIRO_H
#define TABULEIRO_H

#include "Cube.h"
#include <memory>

class Tabuleiro {
public:
    Tabuleiro();
    void drawCasas(Shader& shader) const;
    void drawBase(Shader& shader) const;
    static glm::vec3 centro(int linha, int coluna);
    static bool casa(glm::vec3 ponto, int& linha, int& coluna);

private:
    Cube base;
    std::vector<std::unique_ptr<Cube>> casas;
};

#endif
