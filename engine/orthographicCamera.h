#pragma once
#include "camera.h"

class LIB_API OrthographicCamera : public Camera
{
	private:
		float xmin, xmax, ymin, ymax;
	public:
		OrthographicCamera(char* name, float nearPlane, float farPlane, float xmin, float xmax, float ymin, float ymax);
		virtual ~OrthographicCamera();

		float getXMin();
		void setXMin(float ymin);
		float getXMax();
		void setXMax(float ymax);
		float getYMin();
		void setYMin(float ymin);
		float getYMax();
		void setYMax(float ymax);

		virtual void render(glm::mat4 inverseCamera_M);
};

