#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define LIB_API  // Dummy declaration

class LIB_API Object
{
	private:
		unsigned int id;
		char* name;

		static unsigned int idCounter;
	public:
		Object(char* name);
		virtual ~Object();

		int getId();
		char* getName();

		virtual void render(glm::mat4 inverseCamera_M) = 0;
};

