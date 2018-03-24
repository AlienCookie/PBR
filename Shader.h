#ifndef SHADR
#define SHADR
#include <GL/glew.h>
#include <iostream>
class Shader
{
public:
	static GLuint generateShader(GLenum type, const char* shaderCode);
};

#endif