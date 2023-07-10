#include <iostream>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "renderList.h"
#include "directionalLight.h"
#include "mesh.h"
#include "sphere.h"
#include "shaderGlobals.h"

RenderItem::RenderItem(Mesh* node) : node(node), matrices(node->getMatrices()) {};

RenderList::RenderList(char* name) : Object(name), deltaFrameTime(0.0f) {}

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
	shaders.activateShader(0);

	glDepthFunc(GL_LESS);
	
	for (RenderItem* item : items)
	{
		if (dynamic_cast<const Mesh*>(item->node) != nullptr) {
			Mesh* mesh = (dynamic_cast<Mesh*>(item->node));
			mesh->initVAO();

			unsigned int vao = item->node->getVAO();
			int facesCount = item->node->getFaces().size();

			glBindVertexArray(vao);
			//mesh->pingPongBufferSwap();

			glDrawElementsInstanced(GL_TRIANGLES, facesCount, GL_UNSIGNED_INT, nullptr, item->matrices.size());
			
		}
		else if (dynamic_cast<const DirectionalLight*>(item->node) != nullptr) {
			DirectionalLight* light = (dynamic_cast<DirectionalLight*>(item->node));
			light->render();
		}
	}

	glBindVertexArray(0);
}
