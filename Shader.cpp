#include "stdafx.h"
#include "Shader.h"

GLuint Shader::generateShader(GLenum type,  const char * shaderCode)
{
	GLuint shader;
	GLint compiled;

	shader = glCreateShader(type);

	// Check success
	if (shader == 0) {
		std::cerr << "Unable to create shader" << std::endl;
		return 0;
	}

	// Attach source
	glShaderSource(shader, 1, &shaderCode, NULL);

	// Compile shader
	glCompileShader(shader);

	// Check for compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (compiled == 0)
	{
		std::cerr << "Unable to compile shader " << type <<" "<<GL_VERTEX_SHADER << std::endl;
		glDeleteShader(shader);
		return 0;
	}
	
	return shader;
}
