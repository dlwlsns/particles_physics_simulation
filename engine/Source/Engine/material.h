#pragma once
#include "object.h"

class LIB_API Material : public Object
{
	private:
		glm::vec4 emission;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
		float shininess;
	public:
		Material(char* name, glm::vec4 emission, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess);
		virtual ~Material();

		glm::vec4 getEmission();
		void setEmission(glm::vec4 emission);
		glm::vec4 getAmbient();
		void setAmbient(glm::vec4 ambient);
		glm::vec4 getDiffuse();
		void setDiffuse(glm::vec4 diffuse);
		glm::vec4 getSpecular();
		void setSpecular(glm::vec4 specular);
		float getShininess();
		void setShininess(float shininess);

		virtual void render(glm::mat4 inverseCamera_M);
};

