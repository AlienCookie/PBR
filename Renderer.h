#ifndef RENDR
#define RENDR
#include <GL/glew.h>
#include <list>
#include "GLObject.h"

class Renderer
{
private:
	int screenWidth;
	int screenHeight;

	// objects that will be rendered
	std::list<GLObject*> objectsList;
public:
	Renderer();
	~Renderer();
	void render();
	void addObject(GLObject* object);
	void addAllObjects(std::list<GLObject*>* objects);
	void replaceAllObjects(std::list<GLObject*>* objects);
};

#endif

