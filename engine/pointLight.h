#pragma once
#include "light.h"

class LIB_API PointLight : public Light
{
	private:
		
	public:
		PointLight(char* name, float radius);
		virtual ~PointLight();

		virtual void render(glm::mat4 inverseCamera_M);
};

