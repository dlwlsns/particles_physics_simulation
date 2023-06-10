#pragma once

#include <iostream>
#include <vector>
#include "shader.h"

class ShaderGlobals
{
private:
	std::vector<Shader*> shaderList;
	Shader* activeShader;
public:
	Shader* getShaderByName(const char* name);
	Shader* getShaderById(int id);
	void addShader(Shader* shader);
	void activateShader(int id);
	Shader* getActiveShader();
};

extern ShaderGlobals shaders;


