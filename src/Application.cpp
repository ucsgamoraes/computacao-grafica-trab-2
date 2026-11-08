// Inicialização adaptada de GLFW_Cubo_OO/src/Application.cpp.
#include "Application.h"
#include <iostream>

Application::Application(int w, int h, const std::string& title)
    : width(w), height(h), title(title) {}

Application::~Application() {
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

bool Application::init() {
    glfwSetErrorCallback([](int code, const char* message) {
        std::cerr << "GLFW (" << code << "): " << message << '\n';
    });
#if defined(__linux__) && defined(GLFW_PLATFORM_X11)
    // O GLEW distribuído no Arch usa GLX; no Wayland, utilizar XWayland.
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) return false;

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    const GLenum result = glewInit();
    if (result != GLEW_OK) {
        std::cerr << "Erro ao inicializar GLEW: " << glewGetErrorString(result) << '\n';
        return false;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, [](GLFWwindow* current, int key, int, int action, int) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(current, GLFW_TRUE);
    });
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    return true;
}

void Application::framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}
