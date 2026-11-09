#include "Application.h"
#include "Camera.h"
#include "Interacao.h"
#include "Peca.h"
#include "Tabuleiro.h"
#include "Texture.h"
#include <iostream>

namespace {
glm::mat4 perspective(int width, int height) {
    return glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
}

struct MouseState {
    Camera& camera;
    std::vector<std::unique_ptr<Peca>>& pecas;
    int selecionada = -1;
};

void mouse_button_callback(GLFWwindow* window, int button, int action, int) {
    if (action != GLFW_PRESS) return;
    auto& input = *static_cast<MouseState*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        input.selecionada = -1;
        glfwSetWindowTitle(window, "Xadrez 3D | Clique para selecionar uma peca");
        return;
    }
    if (button != GLFW_MOUSE_BUTTON_LEFT) return;

    double x, y;
    int windowWidth, windowHeight, width, height;
    glfwGetCursorPos(window, &x, &y);
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwGetFramebufferSize(window, &width, &height);
    if (width <= 0 || height <= 0) return;
    Ray ray;
    if (!cursorRay(x, y, windowWidth, windowHeight, width, height,
                   input.camera.view(), perspective(width, height), ray)) return;

    if (input.selecionada < 0) {
        input.selecionada = selecionarPeca(ray, input.pecas);
        if (input.selecionada >= 0) {
            const std::string description = input.pecas[input.selecionada]->descricao();
            std::cout << "Selecionada: " << description << std::endl;
            const std::string title = "Xadrez 3D | " + description + " | Clique no destino; direito cancela";
            glfwSetWindowTitle(window, title.c_str());
        }
    } else {
        int linha, coluna;
        if (!destinoNoTabuleiro(ray, linha, coluna)) {
            std::cout << "Destino fora do tabuleiro; selecao mantida." << std::endl;
            return;
        }
        Peca& peca = *input.pecas[input.selecionada];
        const std::string before = peca.descricao();
        peca.mover(linha, coluna);
        std::cout << "Movimento: " << before << " para " << peca.descricao() << std::endl;
        input.selecionada = -1;
        glfwSetWindowTitle(window, "Xadrez 3D | Clique para selecionar uma peca");
    }
}
}

int main() {
    Application app(1100, 800, "Xadrez 3D | Clique para selecionar uma peca");
    if (!app.init()) return 1;

    Shader shader("shaders/cena.vert", "shaders/cor.frag");
    Shader textureShader("shaders/cena.vert", "shaders/textura.frag");
    if (!shader.isValid() || !textureShader.isValid()) return 1;
    Texture texture("res/images/pedra-28.jpg");
    if (!texture.isValid()) return 1;
    Tabuleiro tabuleiro;
    auto pecas = criarPecas();
    Camera camera;

    GLFWwindow* window = app.getWindow();
    MouseState input{camera, pecas, -1};
    glfwSetWindowUserPointer(window, &input);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    std::cout << "W/A/S/D: caminhar | Setas: olhar | Q/Z: altura | Esc: sair\n"
                 "Clique esquerdo: selecionar, depois mover | Direito: cancelar\n"
                 "Movimentos livres entre casas; sem turnos, captura ou bloqueio de ocupacao.\n"
              << std::flush;
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        const double now = glfwGetTime();
        camera.processInput(window, static_cast<float>(now - lastTime));
        lastTime = now;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        if (width == 0 || height == 0) {
            glfwWaitEvents();
            continue;
        }
        glClearColor(0.11f, 0.14f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        const glm::mat4 view = camera.view(), projection = perspective(width, height);
        textureShader.use();
        textureShader.setMat4("view", view);
        textureShader.setMat4("projection", projection);
        textureShader.setInt("texture1", 0);
        texture.bind();
        tabuleiro.drawBase(textureShader);

        shader.use();
        shader.setBool("selected", false);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        tabuleiro.drawCasas(shader);
        for (std::size_t i = 0; i < pecas.size(); ++i) {
            shader.setBool("selected", input.selecionada == static_cast<int>(i));
            pecas[i]->draw(shader, glm::mat4(1.0f));
        }
        glfwSwapBuffers(window);
    }
    return 0;
}
