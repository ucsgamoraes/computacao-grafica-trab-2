#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Application {
public:
    Application(int w, int h, const std::string& title);
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    bool init();
    GLFWwindow* getWindow() const { return window; }
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:
    GLFWwindow* window = nullptr;
    int width, height;
    std::string title;
};

#endif
