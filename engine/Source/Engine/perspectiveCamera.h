#pragma once
#include "camera.h"

class LIB_API PerspectiveCamera : public Camera
{
private:
	float fov;
	float aspectRatio;
public:
	PerspectiveCamera(char* name, float nearPlane, float farPlane, float fov, float aspectRatio);
	virtual ~PerspectiveCamera();

	float getFov();
	void setFov(float fov);
	float getAspectRatio();
	void setAspectRatio(float aspectRatio);

	virtual void render(glm::mat4 inverseCamera_M);
};

