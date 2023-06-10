#pragma once
#include "node.h"

class LIB_API Camera : public Node
{
	private:
		float nearPlane;
		float farPlane;

		glm::mat4 projection;
	public:
		Camera(char* name, float nearPlane, float farPlane);
		virtual ~Camera();

		glm::mat4 getInverse();

		float getNearPlane();
		void setNearPlane(float nearPlane);
		float getFarPlane();
		void setFarPlane(float farPlane);

		glm::mat4 getProjection();
		void setProjection(glm::mat4 projection);

		virtual void render(glm::mat4 inverseCamera_M) = 0;
};

