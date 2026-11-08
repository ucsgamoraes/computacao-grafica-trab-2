#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
    Camera();
    glm::mat4 view() const;
    void processInput(GLFWwindow* window, float deltaTime);

private:
    glm::vec3 cameraPos{8.0f, 9.0f, -12.0f};
    glm::vec3 cameraFront;
    glm::vec3 cameraUp{0.0f, 1.0f, 0.0f};
    float yaw, pitch;
    void viraCamera(float x, float y);
};

#endif
