#include "stdafx.h"
#include "GLObject.h"
#include "Camera.h"

GLObject::GLObject(
	GLuint _vao, GLuint _program, 
	GLsizei _count,
	GLuint _first,
	GLenum _mode)
{
	vao = _vao;
	program = _program;
	mode = _mode;
	first = _first;
	count = _count;

	angles = { 0.0, 0.0, 0.0 };

	/*
	GLint i;
	GLint uniformsNum;

	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)

	const GLsizei bufSize = 16; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length; // name length

	glGetProgramiv(objectProgram, GL_ACTIVE_UNIFORMS, &uniformsNum);

	for (int i = 0; i < uniformsNum; i++)
	{
		//glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, name);
		glGetActiveAttrib(objectProgram, (GLuint)i, );

		printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
	}

	*/


	locationMV = glGetUniformLocation(program, "MV");
	locationP = glGetUniformLocation(program, "P");
	locationNormalMatrix = glGetUniformLocation(program, "normalMatrix");

	mat4f identityMatrix =
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f ,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	
	xRotationMatrix =
	yRotationMatrix =
	zRotationMatrix =
	scaleTranslateMatrix =
	P =
	MV =
	inverseMV =
	MVP = Matrix<float>(identityMatrix, 4, 4);

	P[0] = 720.f / 1280.f;
	P[10] = -(FAR + NEAR) / (FAR - NEAR);
	P[11] = -2.f * FAR * NEAR / (FAR - NEAR);
	P[14] = -1.f;


	glUniformMatrix4fv(locationP, 1, GL_FALSE, (const GLfloat*)P.transposed());
}

GLObject::~GLObject()
{
	//glDeleteProgram(program);
	//glDeleteVertexArrays(1, &vao);
}

void GLObject::scale(vec3f coefs) {
	scaleTranslateMatrix[0] *= coefs.x;
	scaleTranslateMatrix[5] *= coefs.y;
	scaleTranslateMatrix[10] *= coefs.z;
}

void GLObject::translate(vec3f shift) {
	scaleTranslateMatrix[3] += shift.x;
	scaleTranslateMatrix[7] += shift.y;
	scaleTranslateMatrix[11] += shift.z;
}

void GLObject::rotate(vec3f angl) {
	angles.x += angl.x;
	angles.y += angl.y;
	angles.z += angl.z;

	xRotationMatrix[5]  =  cos(angles.x);
	xRotationMatrix[6]  =  sin(angles.x);
	xRotationMatrix[9]  =  -sin(angles.x);
	xRotationMatrix[10] =  cos(angles.x);

	yRotationMatrix[0] =  cos(angles.y);
	yRotationMatrix[2] = -sin(angles.y);
	yRotationMatrix[8] =  sin(angles.y);
	yRotationMatrix[10] = cos(angles.y);

	zRotationMatrix[0] = cos(angles.z);
	zRotationMatrix[1] = sin(angles.z);
	zRotationMatrix[4] = -sin(angles.z);
	zRotationMatrix[5] = cos(angles.z);
}

void GLObject::updateMVP() {

	MV = scaleTranslateMatrix * zRotationMatrix * yRotationMatrix * xRotationMatrix;
	inverseMV = MV.inverse();
	//MVP = MV * P;
	Matrix<float>normMatr(MV, 3, 3);
	//normMatr = normMatr.inverse();

	glUseProgram(program);
	
	glUniformMatrix4fv(locationMV, 1, GL_FALSE, (const GLfloat*)MV.transposed());
	//glUniformMatrix4fv(locationP, 1, GL_FALSE, (const GLfloat*)mainCamera.getCameraMatrix().transposed());

	glUniformMatrix3fv(locationNormalMatrix, 1, GL_FALSE, (const GLfloat*)normMatr.transposed());
}

void GLObject::setAlbedoMap(GLuint texture)
{
	albedoMap = texture;
}

void GLObject::setNormalMap(GLuint texture)
{
	normalMap = texture;
}

void GLObject::setMetallicMap(GLuint texture)
{
	metallicMap = texture;
}

void GLObject::setRoughnessMap(GLuint texture)
{
	roughnessMap = texture;
}

void GLObject::setAoMap(GLuint texture)
{
	aoMap = texture;
}

void GLObject::setCubemap(GLuint texture)
{
	cubeMap = texture;
}

void GLObject::setOnDraw(bool _onDraw)
{
	onDraw = _onDraw;
}

GLuint GLObject::getMetallicMap()
{
	return metallicMap;
}

GLuint GLObject::getRoughnessMap()
{
	return roughnessMap;
}

GLuint GLObject::getAoMap()
{
	return aoMap;
}

GLuint GLObject::getCubemap()
{
	return cubeMap;
}

Matrix<float> GLObject::getProjection()
{
	return P;
}

Matrix<float> GLObject::getMV()
{
	return MV;
}

Matrix<float> GLObject::getInverseMV()
{
	return inverseMV;
}


Matrix<float> GLObject::getMVP()
{
	return MVP;
}

void GLObject::resetMatrices()
{
	mat4f identityMatrix =
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f ,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	xRotationMatrix =
		yRotationMatrix =
		zRotationMatrix =
		scaleTranslateMatrix = 
		Matrix<float>(identityMatrix, 4, 4);
}

GLuint GLObject::getVAO() {
	return vao; 
}

GLenum GLObject::getMode() { 
	return mode; 
}

GLuint GLObject::getFirst() { 
	return first; 
}

GLuint GLObject::getCount() { 
	return count; 
}

bool GLObject::isOnDraw()
{
	return onDraw;
}

GLuint GLObject::getAlbedoMap()
{
	return albedoMap;
}

GLuint GLObject::getNormalMap()
{
	return normalMap;
}

GLuint GLObject::getProgram() {
	return program;
}
