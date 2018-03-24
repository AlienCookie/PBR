#ifndef CAMER
#define CAMER
#include "Matrix.h"
class Camera

{
private:
	Matrix<float> projectionMatrix;
	Matrix<float> perspectiveMatrix;

	// Camera view options
	const float FOV = 70;
	const float FAR = 9.0;
	const float NEAR = 0.001;

	// Camera position options
	float yaw = 0.f;
	float pitch = 0.f;
	float speed = 0.003f;
	float angle[3];
	float position[4];
public:
	Camera();
	~Camera();
	void move();
	float * getAngle();
	float * getPosition();
	float getYaw();
	void setYaw(float _yaw);
	float getPitch();
	void setPitch(float _pitch);
	Matrix<float> getCameraMatrix();
};


extern Camera mainCamera;

#endif