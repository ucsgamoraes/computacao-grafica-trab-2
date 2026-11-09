#include "Peca.h"
#include "Cube.h"
#include "Primitivas.h"
#include "Tabuleiro.h"
#include <glm/gtc/constants.hpp>
#include <limits>
#include <cmath>

Peca::Peca(int id, TipoPeca tipo, bool branca, int linha, int coluna)
    : id(id), tipo(tipo), branca(branca), linha(linha), coluna(coluna) {
    mover(linha, coluna);
    angle = branca ? 0.0f : 180.0f;
    init();
}

void Peca::init() {
    // A textura só tem os veios em cinza; esta cor dá o tom da madeira.
    const glm::vec3 color = branca ? glm::vec3(0.96f, 0.84f, 0.62f) : glm::vec3(0.40f, 0.22f, 0.12f);
    auto add = [this](std::unique_ptr<Mesh> part, glm::vec3 position, glm::vec3 scale,
                      float angle = 0.0f, glm::vec3 rotation = glm::vec3(1, 0, 0)) {
        part->position = position;
        part->scale = scale;
        part->angle = angle;
        part->rotation = rotation;
        parts.push_back(std::move(part));
    };
    // Enfeite: igual a add(), mas a parte é desenhada com o shader de cor.
    auto enfeite = [this, &add](std::unique_ptr<Mesh> part, glm::vec3 position, glm::vec3 scale) {
        add(std::move(part), position, scale);
        parts.back()->texturizada = false;
    };
    add(std::make_unique<Cilindro>(color), {0,0.06f,0}, {0.72f,0.12f,0.72f});
    add(std::make_unique<Cilindro>(color), {0,0.17f,0}, {0.58f,0.10f,0.58f});

    switch (tipo) {
    case TipoPeca::Peao:
        add(std::make_unique<Cilindro>(color), {0,0.42f,0}, {0.30f,0.40f,0.30f});
        add(std::make_unique<Cilindro>(color), {0,0.65f,0}, {0.43f,0.08f,0.43f});
        enfeite(std::make_unique<Esfera>(color), {0,0.86f,0}, {0.40f,0.40f,0.40f});
        break;
    case TipoPeca::Torre:
        add(std::make_unique<Cilindro>(color), {0,0.49f,0}, {0.46f,0.54f,0.46f});
        add(std::make_unique<Cilindro>(color), {0,0.82f,0}, {0.65f,0.14f,0.65f});
        for (int x : {-1, 1}) for (int z : {-1, 1})
            enfeite(std::make_unique<Cube>(color), {0.21f*x,1.0f,0.21f*z}, {0.19f,0.24f,0.19f});
        break;
    case TipoPeca::Cavalo: {
        add(std::make_unique<Cube>(color), {0,0.53f,-0.02f}, {0.30f,0.67f,0.33f}, 22.0f);
        add(std::make_unique<Cube>(color), {0,0.94f,0.13f}, {0.36f,0.30f,0.44f});
        add(std::make_unique<Cube>(color), {0,0.88f,0.40f}, {0.30f,0.20f,0.30f});
        for (int x : {-1, 1})
            enfeite(std::make_unique<Prisma>(color), {0.11f*x,1.19f,0.02f}, {0.12f,0.25f,0.14f});
        const glm::vec3 eye = branca ? glm::vec3(0.08f) : glm::vec3(0.9f,0.8f,0.6f);
        for (int x : {-1, 1})
            enfeite(std::make_unique<Esfera>(eye), {0.181f*x,0.99f,0.24f}, {0.06f,0.06f,0.06f});
        break;
    }
    case TipoPeca::Bispo:
        add(std::make_unique<Cilindro>(color), {0,0.55f,0}, {0.32f,0.66f,0.32f});
        add(std::make_unique<Cilindro>(color), {0,0.93f,0}, {0.50f,0.10f,0.50f});
        // Duas metades deixam um entalhe visível na mitra.
        for (int x : {-1, 1})
            add(std::make_unique<Prisma>(color), {0.12f*x,1.18f,0}, {0.21f,0.44f,0.34f});
        enfeite(std::make_unique<Esfera>(color), {0,1.46f,0}, {0.13f,0.13f,0.13f});
        break;
    case TipoPeca::Dama:
        add(std::make_unique<Cilindro>(color), {0,0.58f,0}, {0.33f,0.72f,0.33f});
        add(std::make_unique<Cilindro>(color), {0,1.0f,0}, {0.56f,0.12f,0.56f});
        add(std::make_unique<Esfera>(color), {0,1.19f,0}, {0.38f,0.34f,0.38f});
        add(std::make_unique<Cilindro>(color), {0,1.37f,0}, {0.51f,0.10f,0.51f});
        for (int i = 0; i < 6; ++i) {
            const float a = glm::two_pi<float>() * i / 6;
            enfeite(std::make_unique<Esfera>(color), {0.22f*std::cos(a),1.50f,0.22f*std::sin(a)}, {0.14f,0.19f,0.14f});
        }
        break;
    case TipoPeca::Rei:
        add(std::make_unique<Cilindro>(color), {0,0.62f,0}, {0.39f,0.80f,0.39f});
        add(std::make_unique<Cilindro>(color), {0,1.08f,0}, {0.56f,0.12f,0.56f});
        add(std::make_unique<Esfera>(color), {0,1.29f,0}, {0.40f,0.34f,0.40f});
        enfeite(std::make_unique<Cube>(color), {0,1.65f,0}, {0.12f,0.40f,0.12f});
        enfeite(std::make_unique<Cube>(color), {0,1.72f,0}, {0.40f,0.12f,0.12f});
        break;
    }
}

// Cada parte escolhe seu programa: corpo com textura, enfeites com cor.
void Peca::desenhar(Shader& cor, Shader& textura, bool selecionada) const {
    const glm::mat4 model = modelMatrix();
    for (const auto& part : parts) {
        Shader& shader = part->texturizada ? textura : cor;
        shader.use();
        shader.setBool("selected", selecionada);
        part->draw(shader, model);
    }
}

bool Peca::intersectRay(glm::vec3 origin, glm::vec3 direction, float& distance) const {
    distance = std::numeric_limits<float>::infinity();
    bool hit = false;
    const glm::mat4 model = modelMatrix();
    for (const auto& part : parts) {
        float candidate;
        if (part->intersectRay(origin, direction, model, candidate) && candidate < distance) {
            hit = true;
            distance = candidate;
        }
    }
    return hit;
}

void Peca::mover(int novaLinha, int novaColuna) {
    if (novaLinha < 0 || novaLinha >= 8 || novaColuna < 0 || novaColuna >= 8) return;
    linha = novaLinha;
    coluna = novaColuna;
    position = Tabuleiro::centro(linha, coluna);
}

std::string Peca::descricao() const {
    const char* names[] = {"Peao", "Torre", "Cavalo", "Bispo", "Dama", "Rei"};
    return std::string(names[static_cast<int>(tipo)]) + (branca ? " branca " : " preta ") +
           static_cast<char>('A' + coluna) + std::to_string(linha + 1);
}

std::vector<std::unique_ptr<Peca>> criarPecas() {
    std::vector<std::unique_ptr<Peca>> pecas;
    const TipoPeca ordem[] = {TipoPeca::Torre, TipoPeca::Cavalo, TipoPeca::Bispo, TipoPeca::Dama,
                              TipoPeca::Rei, TipoPeca::Bispo, TipoPeca::Cavalo, TipoPeca::Torre};
    for (bool branca : {true, false}) {
        for (int coluna = 0; coluna < 8; ++coluna) {
            pecas.push_back(std::make_unique<Peca>(static_cast<int>(pecas.size()), ordem[coluna], branca,
                                                   branca ? 0 : 7, coluna));
            pecas.push_back(std::make_unique<Peca>(static_cast<int>(pecas.size()), TipoPeca::Peao, branca,
                                                   branca ? 1 : 6, coluna));
        }
    }
    return pecas;
}
