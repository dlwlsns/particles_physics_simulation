#include <iostream>
#include <glm/gtx/string_cast.hpp>

//Ignore freeglut cause it's already included in shader.h that is included in shaderGlobals.h
//#include <GL/freeglut.h>

#include "shaderGlobals.h"
#include "light.h"

unsigned int Light::lightIdCounter = 0;

Light::Light(char* name, float radius)
	: Node(name), lightId(lightIdCounter + LIGHT_INDEX), radius(radius), position(glm::vec4(0.0f)), ambient(glm::vec4(1.0f)), diffuse(glm::vec4(1.0f)), specular(glm::vec4(1.0f)) {

	/*if (lightIdCounter > 8) {
		throw std::runtime_error("Error: Max Light ID count reached.");
	}*/
	lightIdCounter++;

	//glEnable(getLightId());

	//glLightfv(getLightId(), GL_AMBIENT, glm::value_ptr(getAmbient()));
	//glLightfv(getLightId(), GL_DIFFUSE, glm::value_ptr(getDiffuse()));
	//glLightfv(getLightId(), GL_SPECULAR, glm::value_ptr(getSpecular()));

	//glLightf(getLightId(), GL_CONSTANT_ATTENUATION, 1.0f / 10.0f);
	//glLightf(getLightId(), GL_LINEAR_ATTENUATION, 1.0f / 10000.0f);
	//glLightf(getLightId(), GL_QUADRATIC_ATTENUATION, 2.0f/ 1000000.0f);
}

Light::~Light() {
	std::cout << "Deleted light" << std::endl;
}


int Light::getLightId() {
	return lightId;
}

glm::vec4 Light::getPosition() {
	return position;
}

void Light::setPosition(glm::vec3 position) {
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
}

void Light::setPositionW(float w) {
	this->position.w = w;
}

glm::vec4 Light::getAmbient() {
	return ambient;
}

void Light::setAmbient(glm::vec4 ambient) {
	this->ambient = ambient;
}

glm::vec4 Light::getDiffuse() {
	return diffuse;
}

void Light::setDiffuse(glm::vec4 diffuse) {
	this->diffuse = diffuse;
}

glm::vec4 Light::getSpecular() {
	return specular;
}

void Light::setSpecular(glm::vec4 specular) {
	this->specular = specular;
}

float Light::getCutoff() {
	return cutoff;
}

void Light::setCutoff(float cutoff) {
	this->cutoff = cutoff;
}

void Light::render() {
	Shader* current_shader = shaders.getActiveShader();

	int lightPositionLoc = current_shader->getParamLocation("lightPosition");
	int lightAmbientLoc = current_shader->getParamLocation("lightAmbient");
	int lightDiffuseLoc = current_shader->getParamLocation("lightDiffuse");
	int lightSpecularLoc = current_shader->getParamLocation("lightSpecular");

	current_shader->setVec3(lightAmbientLoc, ambient);
	current_shader->setVec3(lightDiffuseLoc, diffuse);
	current_shader->setVec3(lightSpecularLoc, specular);

	current_shader->setVec3(lightPositionLoc, this->getPosition());

	/*glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));
	glm::vec4 pos = inverseCamera_M * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->setPosition(glm::vec3(pos.x, pos.y, pos.z));
	//glLightfv(this->getLightId(), GL_POSITION, glm::value_ptr(this->getPosition()));

	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(glm::vec4(1.0f)));
	//glLoadMatrixf(glm::value_ptr(inverseCamera_M));

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(glm::vec4(1.0f)));
	glLoadMatrixf(glm::value_ptr(inverseCamera_M));
	glutSolidSphere(2.0f, 8, 8);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));*/
}
