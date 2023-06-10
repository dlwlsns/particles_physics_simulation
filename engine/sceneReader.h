#pragma once
#include <cstdio>
#include "material.h"
#include "spotLight.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "light.h"
#include "mesh.h"
#include "node.h"

class SceneReader
{
public:
	FILE* file;
	std::vector<Material*> materials;

	Material* readMaterial(char* data);

	Node* readNode(char* data);

	Light* readLight(char* data);

	Mesh* readMesh(char* data, std::vector<Material*> materials);

	Object* typeSelect(FILE* dat, std::vector<Material*> materials);
};

