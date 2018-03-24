#ifndef TEXTR
#define TEXTR

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

class Texture
{
public:
	static GLuint loadTexturePNG(const char* TextureName);
	static GLuint loadTextureCubemap(std::vector<const char*> faces);
};

#endif