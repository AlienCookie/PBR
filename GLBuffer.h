#ifndef GLBUFF
#define GLBUFF

#include <GL/glew.h>
class GLBuffer
{
private:
	GLuint id;
	GLenum usage;
	GLenum target;
	GLsizeiptr size;

public:
	GLBuffer(
		void* _data,
		GLsizeiptr _size,
		GLenum _target = GL_ARRAY_BUFFER,
		GLenum _usage = GL_STATIC_DRAW);

	~GLBuffer();

	GLuint getId();
	GLenum getTarget();
	GLenum getUsage();
	GLsizeiptr getSize();
};

#endif
