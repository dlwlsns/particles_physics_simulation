#pragma once
#include "mesh.h"

class LIB_API Sphere : public Mesh
{
private:
	void createFace(glm::vec3 normal, int resolution);
public:
	Sphere(char* name, int resolution);
	virtual ~Sphere();

	virtual void render(glm::mat4 inverseCamera_M);
};

