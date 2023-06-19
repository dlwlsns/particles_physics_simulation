#pragma once
#include "light.h"

class LIB_API DirectionalLight : public Light
{
private:

public:
	DirectionalLight(char* name, float radius);
	virtual ~DirectionalLight();

	virtual void render();
};

