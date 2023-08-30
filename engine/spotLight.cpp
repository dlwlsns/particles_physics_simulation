#include "spotLight.h"
#include <iostream>

#include "shaderGlobals.h"
#include <GL/freeglut.h>

SpotLight::SpotLight(char* name, glm::vec3 direction, float radius) : Light(name, radius) {
	this->setPositionW(1.0f);
	this->setCutoff(45.0f);
	this->setDirection(direction);
}

SpotLight::~SpotLight() {
	std::cout << "Deleted spot light" << std::endl;
}

void SpotLight::setCutoff(float cutoff) {
	if (cutoff > 90.0f)
		cutoff = 90.0f;
	if (cutoff < 0.0f)
		cutoff = 0.0f;

	this->Light::setCutoff(cutoff);
}

glm::vec3 SpotLight::getDirection() {
	return direction;
}

void SpotLight::setDirection(glm::vec3 direction) {
	this->direction = direction;
}

void SpotLight::render(glm::mat4 inverseCamera) {
	Light::render();
};