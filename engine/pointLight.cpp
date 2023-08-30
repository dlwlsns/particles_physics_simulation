#include <iostream>
#include <GL/freeglut.h>

#include "pointLight.h"

PointLight::PointLight(char* name, float radius) : Light(name, radius) {
	this->setPositionW(1.0f);
	this->setCutoff(180.0f);

	float cutoff = getCutoff();
}

PointLight::~PointLight() {
	std::cout << "Deleted point light" << std::endl;
}

void PointLight::render(glm::mat4 inverseCamera) {
	Light::render();
};