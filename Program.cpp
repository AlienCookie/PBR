#include "stdafx.h"
#include "Program.h"


GLuint Program::generateProgram(GLuint* shaders, int size)
{
	GLuint program;
	GLint linked;

	// Creating program instance
	program = glCreateProgram();

	// Attaching shaders
	for (int i = 0; i < size; i++)
	{
		glAttachShader(program, shaders[i]);
	}

	glLinkProgram(program);

	// Checking linking status
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (linked == 0) {
		std::cerr << "Unable to link program" << std::endl;
		glDeleteProgram(program);
		return 0;
	}

	return program;
}
