#pragma once

#include "object.h"

// GLM:
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Glew (include it before GL.h):
#include <GL/glew.h>

// FreeGLUT:
#include <GL/freeglut.h>

// C/C++:
#include <iostream>

class LIB_API Shader : public Object
{
public:

	// Constants:
	static const unsigned int MAX_LOGSIZE = 4096;  ///< Max output size in char for a shader log

	 // Enums:
	enum ///< Kind of shader
	{
		TYPE_UNDEFINED = 0,
		TYPE_VERTEX,
		TYPE_FRAGMENT,
		TYPE_PROGRAM,
		TYPE_LAST
	};

	// Const/dest:	 
	Shader(char* name);
	~Shader();

	// Get/set:
	int getParamLocation(const char* name);
	inline void setMatrix(int param, const glm::mat4& mat)
	{
		glUniformMatrix4fv(param, 1, GL_FALSE, glm::value_ptr(mat));
	}
	inline void setMatrix3(int param, const glm::mat3& mat)
	{
		glUniformMatrix3fv(param, 1, GL_FALSE, glm::value_ptr(mat));
	}
	inline void setFloat(int param, float value)
	{
		glUniform1f(param, value);
	}
	inline void setInt(int param, int value)
	{
		glUniform1i(param, value);
	}
	inline void setVec3(int param, const glm::vec3& vect)
	{
		glUniform3fv(param, 1, glm::value_ptr(vect));
	}
	inline void setVec4(int param, const glm::vec4& vect)
	{
		glUniform4fv(param, 1, glm::value_ptr(vect));
	}

	// Accessing data:
	bool loadFromMemory(int kind, const char* data);
	bool loadFromFile(int kind, const char* filename);
	bool build(Shader* vertexShader, Shader* fragmentShader);

	inline void bind(int location, const char* attribName)
	{
		glBindAttribLocation(glId, location, attribName);
	}

	// Rendering:				
	bool use(void* data = nullptr);


	virtual void render(glm::mat4 inverseCamera_M);

	// OGL id:
	GLuint glId;

	///////////	 
private:	//
///////////					

   // Object kind:
	int type;

	
};

