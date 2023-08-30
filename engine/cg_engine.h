#pragma once

// Generic info:
#define LIB_NAME      "CG Engine v0.1a"  ///< Library credits
#define LIB_VERSION   10                 ///< Library version (divide by 10)

#ifdef _WINDOWS 	
   // Export API:
   // Specifies i/o linkage (VC++ spec):
   #ifdef CG_ENGINE_EXPORTS
		#define LIB_API __declspec(dllexport)
   #else
		#define LIB_API __declspec(dllimport)
   #endif      	
#else // Under Linux
   #define LIB_API  // Dummy declaration
#endif

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/packing.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
// Engine
#include "object.h"
#include "node.h"
#include "mesh.h"
#include "sphere.h"
#include "perspectiveCamera.h"
#include "orthographicCamera.h"
#include "pointLight.h"
#include "directionalLight.h"


/////////////
// CLASSES //
/////////////


class LIB_API CgEngine
{
	private:
		bool initFlag;

		float border = 0.5f;
		int cellCount = 1;
		glm::vec3 gridCenter = glm::vec3(0);

		// Singleton
		static CgEngine* istance;
		CgEngine();

		unsigned int ssboGrid;
		unsigned int ssboGridCounter;

		std::vector<int> cells;
		std::vector<unsigned int> counters;

		
	public:   
		static CgEngine* getIstance();

		bool init(int argc, char* argv[]);
		void parse(Node* scene);
		void run();
		bool free();     

		void setKeyboardCallback(void (*func)(unsigned char, int, int));
		void setSpecialCallback(void (*func)(int, int, int));
		void setIdleCallback(void (*func)());
		unsigned int getElapsedTime();

		void cameraRotation();
		void toggleWireframe();
		void addGuiText(char* text);
		void updateGrid();
		void updateUniforms();

		void setBorderDimension(float dimension);
		void setCellCount(unsigned int cells);
		void setGridCenter(glm::vec3 center);
};
