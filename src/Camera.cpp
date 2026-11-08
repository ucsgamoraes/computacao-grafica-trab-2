// Base: Câmera Virtual.pdf, slides 31–33 e 45–49.
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

Camera::Camera() {
    cameraFront = glm::normalize(glm::vec3(0, 0.3f, 0) - cameraPos);
    yaw = glm::degrees(std::atan2(cameraFront.z, cameraFront.x));
    pitch = glm::degrees(std::asin(cameraFront.y));
}

glm::mat4 Camera::view() const {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::viraCamera(float x, float y) {
    yaw += x;
    pitch = std::max(-89.0f, std::min(89.0f, pitch + y));
    glm::vec3 direction;
    direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    direction.y = std::sin(glm::radians(pitch));
    direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void Camera::processInput(GLFWwindow* window, float deltaTime) {
    const float dt = std::min(deltaTime, 0.1f);
    auto pressed = [window](int key) { return glfwGetKey(window, key) == GLFW_PRESS; };
    viraCamera((pressed(GLFW_KEY_RIGHT) - pressed(GLFW_KEY_LEFT)) * 65.0f * dt,
               (pressed(GLFW_KEY_UP) - pressed(GLFW_KEY_DOWN)) * 65.0f * dt);
    const glm::vec3 forward = glm::normalize(glm::vec3(cameraFront.x, 0, cameraFront.z));
    const glm::vec3 right = glm::normalize(glm::cross(forward, cameraUp));
    glm::vec3 direction = forward * static_cast<float>(pressed(GLFW_KEY_W) - pressed(GLFW_KEY_S)) +
                          right * static_cast<float>(pressed(GLFW_KEY_D) - pressed(GLFW_KEY_A));
    if (glm::length(direction) > 0.0f) cameraPos += glm::normalize(direction) * 4.0f * dt;
    cameraPos.y += (pressed(GLFW_KEY_Q) - pressed(GLFW_KEY_Z)) * 4.0f * dt;
}
