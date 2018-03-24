#ifndef GLOBJECT
#define GLOBJECT

#include <GL/glew.h>
#include "Matrix.h"
#include "FileManager.h"
#include <map>

//typedef float vec4f[4];
//typedef float vec3f[3];
//typedef float vec2f[2];

typedef float mat4f[16];

/* Object that contains 
* only drawing information
* and transformation matrices
*/

const float FAR = 5.0;
const float NEAR = 0.0001;

class GLObject
{
private:
	GLuint vao;
	GLuint program;

	GLuint albedoMap;
	GLuint normalMap;
	GLuint metallicMap;
	GLuint roughnessMap;
	GLuint aoMap;
	GLuint cubeMap;

	std::map<std::string, unsigned> uniforms;

	// drawing params
	bool onDraw;
	GLenum mode;
	GLuint first;
	GLsizei count;

	GLuint locationP;
	GLuint locationMV;
	GLuint locationNormalMatrix;

protected:
	// Rotation angles
	vec3f angles;

	// Projection matrix
	Matrix<float> P;

	// Model View matrix
	Matrix<float> MV;
	Matrix<float> inverseMV;
	Matrix<float> MVP;

	// Transofrmation matrices 
	Matrix<float> xRotationMatrix;
	Matrix<float> yRotationMatrix;
	Matrix<float> zRotationMatrix;
	Matrix<float> scaleTranslateMatrix;

public:
	GLObject(
	GLuint vao,
	GLuint program,
	GLsizei count,
	GLuint first = 0,
	GLenum mode = GL_TRIANGLES);

	~GLObject();

	void scale(vec3f coefs);
	void translate(vec3f coefs);
	void rotate(vec3f angl);

	void updateMVP();

	void setAlbedoMap(GLuint texture);
	void setNormalMap(GLuint texture);
	void setMetallicMap(GLuint texture);
	void setRoughnessMap(GLuint texture);
	void setAoMap(GLuint texture);
	void setCubemap(GLuint texture);

	void setOnDraw(bool _onDraw);

	GLuint getVAO();
	GLuint getProgram();
	GLenum getMode();
	GLuint getFirst();
	GLuint getCount();

	bool isOnDraw();

	GLuint getAlbedoMap();
	GLuint getNormalMap();
	GLuint getMetallicMap();
	GLuint getRoughnessMap();
	GLuint getAoMap();
	GLuint getCubemap();

	Matrix<float> getProjection();
	Matrix<float> getMV();
	Matrix<float> getInverseMV();
	Matrix<float> getMVP();

	void resetMatrices();
};

#endif