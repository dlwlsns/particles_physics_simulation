#pragma once
#include "mesh.h"
#include "shaderGlobals.h"
#include <vector>
#include <typeinfo>


struct RenderItem {
	Mesh* node;
	std::vector<glm::vec4> matrices;

	RenderItem(Mesh* node);
};

class RenderList : public Object
{
	private:
		std::vector<RenderItem*> items;
	public:
		float deltaFrameTime;

		RenderList(char* name);
		virtual ~RenderList();

		void addItem(RenderItem* item);
		void removeitem(unsigned int id);
		RenderItem* get(int index);
		void set(int index, RenderItem* item);
		void empty();
		int size();
		void sort();

		void render(glm::mat4 inverseCamera_M);
};

