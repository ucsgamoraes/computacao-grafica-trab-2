#ifndef PECA_H
#define PECA_H

#include "Mesh.h"
#include <memory>

enum class TipoPeca { Peao, Torre, Cavalo, Bispo, Dama, Rei };

// Composição de partes adaptada de Banrisul, no exemplo GLFW_Cubo_OO.
class Peca : public Object {
public:
    Peca(int id, TipoPeca tipo, bool branca, int linha, int coluna);
    void desenhar(Shader& cor, Shader& textura, bool selecionada) const;
    bool intersectRay(glm::vec3 origin, glm::vec3 direction, float& distance) const;
    void mover(int novaLinha, int novaColuna);
    std::string descricao() const;

    int id;
    TipoPeca tipo;
    bool branca;
    int linha, coluna;

private:
    std::vector<std::unique_ptr<Mesh>> parts;
    void init();
};

std::vector<std::unique_ptr<Peca>> criarPecas();

#endif
