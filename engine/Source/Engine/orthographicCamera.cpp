#include "orthographicCamera.h"
#include <iostream>

OrthographicCamera::OrthographicCamera(char* name, float nearPlane, float farPlane, float xmin, float xmax, float ymin, float ymax) : Camera(name, nearPlane, farPlane), xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax) {}

OrthographicCamera::~OrthographicCamera() {
    std::cout << "Deleted ortho camera" << std::endl;
}

float OrthographicCamera::getXMin() {
    return xmin;
}

void OrthographicCamera::setXMin(float xmin) {
    this->xmin = xmin;
}

float OrthographicCamera::getXMax() {
    return xmax;
}

void OrthographicCamera::setXMax(float xmax) {
    this->xmax = xmax;
}

float OrthographicCamera::getYMin() {
    return ymin;
}

void OrthographicCamera::setYMin(float ymin) {
    this->ymin = ymin;
}

float OrthographicCamera::getYMax() {
    return ymax;
}

void OrthographicCamera::setYMax(float ymax) {
    this->ymax = ymax;
}

void OrthographicCamera::render(glm::mat4 inverseCamera) {}