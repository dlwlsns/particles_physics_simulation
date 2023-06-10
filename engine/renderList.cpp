#include <iostream>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <GL/glew.h>
#include <GL/freeglut.h>


#include "renderList.h"
#include "light.h"
#include "mesh.h"
#include "shaderGlobals.h"

RenderItem::RenderItem(Object* object, glm::mat4 matrix) : object(object), matrix(matrix) {};

RenderList::RenderList(char* name) : Object(name) {}

RenderList::~RenderList() {
	std::cout << "Deleted render list" << std::endl;
}

void RenderList::addItem(RenderItem item) {
	items.push_back(item);
}

void RenderList::removeitem(unsigned int id) {
	items.erase(items.begin()+id);
}

RenderItem RenderList::get(int index) {
	return items[index];
}

void RenderList::set(int index, RenderItem item) {
	items[index] = item;
}

void RenderList::empty() {
	items.clear();
}

int RenderList::size() {
	return items.size();
}

void RenderList::sort() {
	std::sort(items.begin(), items.end(), [](const RenderItem obj1, const RenderItem obj2) -> int {
		return (((dynamic_cast<Light*>(obj1.object) != nullptr) ? 1 : 0) > ((dynamic_cast<Light*>(obj2.object) != nullptr ? 1 : 0)));
	});
}

void RenderList::render(glm::mat4 inverseCamera_M) {
	glDepthFunc(GL_LESS);
	Shader* current_shader = shaders.getActiveShader();
	for (RenderItem item : items)
	{
		current_shader->setMatrix(current_shader->getParamLocation("modelview"), inverseCamera_M * item.matrix);
		glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(item.matrix));
		current_shader->setMatrix3(current_shader->getParamLocation("normalMatrix"), normalMatrix);

		item.object->render(inverseCamera_M * item.matrix);
	}
}
