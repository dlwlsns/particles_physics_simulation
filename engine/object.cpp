#include <stdexcept>
#include <iostream>

#include "object.h"

unsigned int Object::idCounter = 0;

Object::Object(char* name) : name(name), id(idCounter) {
	if(idCounter > INT_MAX) {
		throw std::runtime_error("Error: Max ID count reached.");
	}

	idCounter++;
}

Object::~Object() {
	std::cout << "Deleted object" << std::endl;
}

int Object::getId() {
	return id;
}

char* Object::getName() {
	return name;
}