#include "stdafx.h"
#include "GLBuffer.h"
#include <iostream>


GLBuffer::GLBuffer(void* _data, GLsizeiptr _size,
	GLenum _target,
	GLenum _usage)
{
	glGenBuffers(1, &id);
	target = _target;
	usage = _usage;
	size = _size;
	
	glBindBuffer(target, id);
	glBufferData(target, size, _data, usage);
}


GLBuffer::~GLBuffer()
{
	//glDeleteBuffers(1, &id);
}

GLuint GLBuffer::getId()
{
	return id;
}

GLenum GLBuffer::getTarget()
{
	return target;
}

GLenum GLBuffer::getUsage()
{
	return usage;
}


GLsizeiptr GLBuffer::getSize()
{
	return size;
}


