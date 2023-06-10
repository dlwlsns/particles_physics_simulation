//Ignore freeglut cause it's already included in shader.h that is included in shaderGlobals.h
//#include <GL/freeglut.h>
#include <iostream>

#include "material.h"
#include "shaderGlobals.h"

Material::Material(char* name, glm::vec4 emission, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess)
	: Object(name), emission(emission), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

Material::~Material() {
	std::cout << "Deleted material" << std::endl;
}

glm::vec4 Material::getEmission() {
	return emission;
}

void Material::setEmission(glm::vec4 emission) {
	this->emission = emission;
}

glm::vec4 Material::getAmbient() {
	return ambient;
}

void Material::setAmbient(glm::vec4 ambient) {
	this->ambient = ambient;
}

glm::vec4 Material::getDiffuse() {
	return diffuse;
}

void Material::setDiffuse(glm::vec4 diffuse) {
	this->diffuse = diffuse;
}

glm::vec4 Material::getSpecular() {
	return specular;
}

void Material::setSpecular(glm::vec4 specular) {
	this->specular = specular;
}

float Material::getShininess() {
	return shininess;
}

void Material::setShininess(float shininess) {
	this->shininess = shininess;
}

void Material::render(glm::mat4 inverseCamera_M) {
	Shader* current_shader = shaders.getActiveShader();
	
	int matEmissionLoc = current_shader->getParamLocation("matEmission");
	int matAmbientLoc = current_shader->getParamLocation("matAmbient");
	int matDiffuseLoc = current_shader->getParamLocation("matDiffuse");
	int matSpecularLoc = current_shader->getParamLocation("matSpecular");
	int matShininessLoc = current_shader->getParamLocation("matShininess");

	current_shader->setVec3(matEmissionLoc, emission);
	current_shader->setVec3(matAmbientLoc, ambient);
	current_shader->setVec3(matDiffuseLoc, diffuse);
	current_shader->setVec3(matSpecularLoc, specular);
	current_shader->setFloat(matShininessLoc, shininess);
}