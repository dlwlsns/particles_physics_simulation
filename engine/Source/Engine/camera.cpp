#include "camera.h"
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "shaderGlobals.h"

Camera::Camera(char* name, float nearPlane, float farPlane) : Node(name), nearPlane(nearPlane), farPlane(farPlane), projection(glm::mat4(1.0f)) {}

Camera::~Camera() {
    std::cout << "Deleted camera" << std::endl;
}

glm::mat4 Camera::getInverse() {
	return glm::inverse(this->getWorldCoordinates());
}

float Camera::getNearPlane() {
	return nearPlane;
}

void Camera::setNearPlane(float nearPlane) {
	this->nearPlane = nearPlane;
}

float Camera::getFarPlane() {
	return farPlane;
}

void Camera::setFarPlane(float farPlane) {
	this->farPlane = farPlane;
}

glm::mat4 Camera::getProjection() {
	return projection;
}

void Camera::setProjection(glm::mat4 projection) {
	this->projection = projection;
}

/**
 * This callback is invoked each time a keyboard key is pressed.
 * @param key key pressed id
 * @param mouseX mouse X coordinate
 * @param mouseY mouse Y coordinate
 */
void Camera::move(void* window)
{
    GLFWwindow* w = static_cast<GLFWwindow*>(window);

    const float cameraSpeed = 0.2f; // adjust accordingly
    bool checkPress = false;

    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
        this->appendMatrix(glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, cameraSpeed))));
        checkPress = true;
    }

    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
        this->appendMatrix(glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -cameraSpeed))));
        checkPress = true;
    }

    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
        this->appendMatrix(glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(cameraSpeed, 0.0f, 0.0f))));
        checkPress = true;
    }

    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
        this->appendMatrix(glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(-cameraSpeed, 0.0f, 0.0f))));
        checkPress = true;
    }

    if (glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS) {
        this->appendMatrix(glm::inverse(glm::rotate(glm::mat4(1.0f), glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f))));
        checkPress = true;
    }

    if (glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS) {
        this->appendMatrix(glm::inverse(glm::rotate(glm::mat4(1.0f), glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f))));
        checkPress = true;
    }

    if (glfwGetKey(w, GLFW_KEY_LEFT) == GLFW_PRESS) {
        this->appendMatrix(glm::inverse(glm::rotate(glm::mat4(1.0f), glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f))));
        checkPress = true;
    }

    if (glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        this->appendMatrix(glm::inverse(glm::rotate(glm::mat4(1.0f), glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f))));
        checkPress = true;
    }

    if (checkPress) {
        Shader* current_shader = shaders.getShaderById(0);
        current_shader->setMatrix(current_shader->getParamLocation("projection"), this->getProjection());
        glUniformMatrix4fv(current_shader->getParamLocation("invCamera"), 1, GL_FALSE, glm::value_ptr(this->getInverse()));
    }
}