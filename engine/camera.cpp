#include "camera.h"
#include <iostream>

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