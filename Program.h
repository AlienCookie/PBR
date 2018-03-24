#ifndef G_PROG
#define G_PROG

#include <GL/glew.h>
#include <iostream>

class Program
{
public:
	static GLuint generateProgram(GLuint* shades, int size);
};

#endif