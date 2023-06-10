#include "shaderGlobals.h";

Shader* ShaderGlobals::getShaderByName(const char* name) {
	//auto it = std::find_if(shaderList.begin(), shaderList.end(), [name](const Shader& s) {return s.getName() == myString; })
	return nullptr;
}
Shader* ShaderGlobals::getShaderById(int id) {
	return shaderList[id];
}
void ShaderGlobals::addShader(Shader* shader) {
	shaderList.push_back(shader);
}

void ShaderGlobals::activateShader(int id) {
	Shader* s = shaderList[id];
	activeShader = s;
	s->use();
}

Shader* ShaderGlobals::getActiveShader() {
	return activeShader;
}