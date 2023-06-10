#pragma once
#include "node.h"

#define LIGHT_INDEX 16384

class LIB_API Light : public Node
{
	private:
		unsigned int lightId;
		glm::vec4 position;
		float radius;

		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;

		float cutoff;

		static unsigned int lightIdCounter;
	public:
		Light(char* name, float radius);
		virtual ~Light();

		virtual void render(glm::mat4 inverseCamera_M);

		int getLightId();

		glm::vec4 getPosition();
		void setPosition(glm::vec3 position);
		void setPositionW(float w);

		glm::vec4 getAmbient();
		void setAmbient(glm::vec4 ambient);
		glm::vec4 getDiffuse();
		void setDiffuse(glm::vec4 diffuse);
		glm::vec4 getSpecular();
		void setSpecular(glm::vec4 specular);

		float getCutoff();
		virtual void setCutoff(float cutoff);
};

