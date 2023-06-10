#include "spotLight.h"
#include <iostream>

#include "shaderGlobals.h"
#include <GL/freeglut.h>

SpotLight::SpotLight(char* name, glm::vec3 direction, float radius) : Light(name, radius) {
	this->setPositionW(1.0f);
	this->setCutoff(45.0f);
	this->setDirection(direction);

	//glLightfv(this->getLightId(), GL_POSITION, glm::value_ptr(this->getPosition()));
	//float cutoff = getCutoff();
	//glLightfv(this->getLightId(), GL_SPOT_CUTOFF, &cutoff);
	//glLightfv(this->getLightId(), GL_SPOT_DIRECTION, glm::value_ptr(this->getDirection()));
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
	/*shaders.activateShader(1);

	Shader* current_shader = shaders.getActiveShader();
	int lightDirection = current_shader->getParamLocation("lightDirection");
	int lightConeAngle = current_shader->getParamLocation("lightConeAngle");

	current_shader->setVec3(lightDirection, getDirection());
	current_shader->setFloat(lightConeAngle, getCutoff());
	*/
	Light::render(inverseCamera);
	//glLightfv(this->getLightId(), GL_SPOT_DIRECTION, glm::value_ptr(this->getDirection()));
};