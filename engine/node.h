#pragma once
#include "object.h"
#include <vector>

class LIB_API Node : public Object
{
	private:
		Node* parent;
		std::vector<Node*> childrens;

		glm::mat4 objectCoordinates;
		glm::mat4 worldCoordinates;

		void setWorldCoordinates(glm::mat4 worldCoordinates);

		void setParent(Node* parent);
		void updateChildren();

	public:
		Node(char* name);
		virtual ~Node();

		void addChild(Node* child);
		Node* removeChild(unsigned int index);
		int getChildrenCount();
		Node* getChild(unsigned int listIndex);

		Node* getParent();

		void setObjectCoordinates(glm::mat4 objectCoordinates);
		glm::mat4 getObjectCoordinates();
		glm::mat4 getWorldCoordinates();

		void appendMatrix(glm::mat4 matrix);
		void insertMatrix(glm::mat4 matrix);

		virtual void render(glm::mat4 inverseCamera_M);
};

