#include "perspectiveCamera.h"
#include <iostream>

PerspectiveCamera::PerspectiveCamera(char* name, float nearPlane, float farPlane, float fov, float aspectRatio) : Camera(name, nearPlane, farPlane), fov(fov), aspectRatio(aspectRatio) {}

PerspectiveCamera::~PerspectiveCamera() {
    std::cout << "Deleted proj camera" << std::endl;
}

float PerspectiveCamera::getFov() {
    return fov;
}

void PerspectiveCamera::setFov(float fov) {
    this->fov = fov;
}

float PerspectiveCamera::getAspectRatio() {
    return aspectRatio;
}

void PerspectiveCamera::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
}

void PerspectiveCamera::render(glm::mat4 inverseCamera) {}