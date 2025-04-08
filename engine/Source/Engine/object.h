#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

class LIB_API Object
{
	private:
		unsigned int id;
		char* name;

		static unsigned int idCounter;
	public:
		Object(char* name);
		virtual ~Object();

		int getId();
		char* getName();

		virtual void render(glm::mat4 inverseCamera_M) = 0;
};

