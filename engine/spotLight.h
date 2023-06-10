#pragma once
#include "light.h"

class LIB_API SpotLight : public Light
{
	private:
		glm::vec3 direction;
	public:
		SpotLight(char* name, glm::vec3 direction, float radius);
		virtual ~SpotLight();

		void setCutoff(float cutoff);

		glm::vec3 getDirection();
		void setDirection(glm::vec3 direction);

		virtual void render(glm::mat4 inverseCamera_M);
};

