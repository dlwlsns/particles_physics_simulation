#include <iostream>
#include <glm/gtx/string_cast.hpp>

//Ignore freeglut cause it's already included in shader.h that is included in shaderGlobals.h
//#include <GL/freeglut.h>

#include "shaderGlobals.h"
#include "light.h"

unsigned int Light::lightIdCounter = 0;

Light::Light(char* name, float radius)
	: Node(name), lightId(lightIdCounter), radius(radius), position(glm::vec4(0.0f)), ambient(glm::vec4(1.0f)), diffuse(glm::vec4(1.0f)), specular(glm::vec4(1.0f)) {

	lightIdCounter++;
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
}
