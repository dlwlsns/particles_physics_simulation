#pragma once
#include "node.h"
#include "shaderGlobals.h"
#include <vector>
#include <typeinfo>


struct RenderItem {
	Object* object;
	glm::mat4 matrix;

	RenderItem(Object* object, glm::mat4 matrix);
};

class RenderList : public Object
{
	private:
		std::vector<RenderItem> items;
	public:
		RenderList(char* name);
		virtual ~RenderList();

		void addItem(RenderItem item);
		void removeitem(unsigned int id);
		RenderItem get(int index);
		void set(int index, RenderItem item);
		void empty();
		int size();
		void sort();

		void render(glm::mat4 inverseCamera_M);
};

