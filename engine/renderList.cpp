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

RenderItem::RenderItem(Mesh* node) : node(node) {};

RenderList::RenderList(char* name) : Object(name) {}

RenderList::~RenderList() {
	std::cout << "Deleted render list" << std::endl;
}

void RenderList::addItem(RenderItem* item) {
	items.push_back(item);
}

void RenderList::removeitem(unsigned int id) {
	items.erase(items.begin()+id);
}

RenderItem* RenderList::get(int index) {
	return items[index];
}

void RenderList::set(int index, RenderItem* item) {
	items[index] = item;
}

void RenderList::empty() {
	items.clear();
}

int RenderList::size() {
	return items.size();
}

void RenderList::sort() {
	/*std::sort(items.begin(), items.end(), [](const RenderItem obj1, const RenderItem obj2) -> int {
		return (((dynamic_cast<Light*>(obj1.node) != nullptr) ? 1 : 0) > ((dynamic_cast<Light*>(obj2.node) != nullptr ? 1 : 0)));
	});*/
}

void RenderList::render(glm::mat4 inverseCamera_M) {
	glDepthFunc(GL_LESS);
	Shader* current_shader = shaders.getActiveShader();

	glUniformMatrix4fv(current_shader->getParamLocation("invCamera"), 1, GL_FALSE, glm::value_ptr(inverseCamera_M));

	for (RenderItem* item : items)
	{
		if (dynamic_cast<const Mesh*>(item->node) != nullptr) {
			(dynamic_cast<Mesh*>(item->node))->initVAO();
		}

		unsigned int vao = item->node->getVAO();
		int facesCount = item->node->getFaces().size();
		
		for (int i = 0; i < item->matrices.size(); i++) {
			
			//glUniformMatrix4fv(current_shader->getParamLocation(("coords[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(item->matrices[i]));
			current_shader->setMatrix(current_shader->getParamLocation("temp"), item->matrices[i]);
			glUniformMatrix3fv(current_shader->getParamLocation("normalMatrix"), 1, GL_TRUE, glm::value_ptr(glm::inverse(glm::mat3(item->matrices[i]))));

			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, facesCount, GL_UNSIGNED_INT, nullptr);
		}
	}
}
