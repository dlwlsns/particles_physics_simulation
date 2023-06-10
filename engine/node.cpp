#include "node.h"
#include <iostream>

Node::Node(char* name) : Object(name), parent(nullptr), objectCoordinates(glm::mat4(1.0f)), worldCoordinates(glm::mat4(1.0f)) {}

Node::~Node() {
	for (int i = 0; i < getChildrenCount(); i++) {
		delete childrens[i];
	}
	std::cout << "Deleted node" << std::endl;
}


void Node::addChild(Node* child) {
	this->childrens.push_back(child);
	child->setParent(this);
}

Node* Node::removeChild(unsigned int index) {
	if (index >= this->childrens.size() || index < 0) return nullptr;

	Node* dum = this->childrens[index];
	dum->setParent(nullptr);
	this->childrens.erase(this->childrens.begin() + index);

	return dum;
}

int Node::getChildrenCount() {
	return static_cast<int>(this->childrens.size());
}

Node* Node::getChild(unsigned int listIndex) {
	if (listIndex >= getChildrenCount())
		return nullptr;

	return this->childrens[listIndex];
}


void Node::setParent(Node* parent) {
	this->parent = parent;

	if (parent != nullptr) {
		glm::mat4 result = this->getParent()->getWorldCoordinates() * this->getObjectCoordinates();
		this->setWorldCoordinates(result);
	} else {
		this->setWorldCoordinates(glm::mat4(1.0f));
	}

	this->updateChildren();
}

Node* Node::getParent() {
	return this->parent;
}

void Node::setObjectCoordinates(glm::mat4 matrix) {
	this->objectCoordinates = matrix;

	if (this->getParent() != nullptr) {
		glm::mat4 result = this->getParent()->getWorldCoordinates() * this->getObjectCoordinates();
		this->setWorldCoordinates(result);
	}
}

glm::mat4 Node::getObjectCoordinates() {
	return this->objectCoordinates;
}

void Node::setWorldCoordinates(glm::mat4 worldCoordinates) {
	this->worldCoordinates = worldCoordinates;
}

glm::mat4 Node::getWorldCoordinates() {
	if (this->getParent() == nullptr) {
		return this->objectCoordinates;
	}

	return this->worldCoordinates;
}

void Node::updateChildren() {
	for (Node* child : childrens) {
		child->setParent(this);
		child->updateChildren();
	}
}

void Node::appendMatrix(glm::mat4 matrix) {
	this->objectCoordinates = this->objectCoordinates * matrix;

	if (this->getParent() != nullptr) {
		glm::mat4 result = this->getParent()->getWorldCoordinates() * this->getObjectCoordinates();
		this->setWorldCoordinates(result);
	}

	this->updateChildren();
}

void Node::insertMatrix(glm::mat4 matrix) {
	this->objectCoordinates = matrix * this->objectCoordinates;

	if (this->getParent() != nullptr) {
		glm::mat4 result = this->getParent()->getWorldCoordinates() * this->getObjectCoordinates();
		this->setWorldCoordinates(result);
	}

	this->updateChildren();
}

void Node::render(glm::mat4 inverseCamera) {};
