#include "stdafx.h"
#include "Camera.h"
#include "ActionManager.h"
#include <cmath>

static const float PI = 3.141592;
static const float RADIAN_COEFFICIENT = (float)PI / 180.f;

Camera mainCamera;

Camera::Camera()
{

	float baseMatrix[] =
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f ,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	perspectiveMatrix = projectionMatrix = Matrix<float>(baseMatrix, 4, 4);

	// Calculation camera screen relative params
	float aspectRatio = (float)1280 / (float)720;
	float y_scale = (float)(1.f / tan((FOV / 2.f)*RADIAN_COEFFICIENT)) * aspectRatio;
	float x_scale = y_scale / aspectRatio;
	float frustum_length = FAR - NEAR;

	// Setting up camera perspective matrix
	perspectiveMatrix[0] = x_scale;
	perspectiveMatrix[5] = y_scale;
	perspectiveMatrix[10] = -((FAR + NEAR) / frustum_length);
	perspectiveMatrix[11] = -((2.f * NEAR * FAR) / frustum_length);
	perspectiveMatrix[14] = -1.0f;
	perspectiveMatrix[15] = 0.0f;

	// Setting up camera starting position
	position[0] = 0.0f;
	position[1] = 0.0f;
	position[2] = 0.0f;

}


Camera::~Camera()
{
}

Matrix<float> createViewMatrix(float pitch, float yaw, float* position) {
	Matrix<float> viewMatrix(4, 4);

	viewMatrix[0] = 1.f;
	viewMatrix[5] = 1.f;
	viewMatrix[10] = 1.f;
	viewMatrix[15] = 1.f;

	float xAngle = pitch * RADIAN_COEFFICIENT;
	Matrix<float> xRotatioMatrix = viewMatrix;
	xRotatioMatrix[5] = cos(xAngle);
	xRotatioMatrix[6] = sin(xAngle);
	xRotatioMatrix[9] = -sin(xAngle);
	xRotatioMatrix[10] = cos(xAngle);

	float yAngle = -yaw * RADIAN_COEFFICIENT;
	Matrix<float> yRotatioMatrix = viewMatrix;
	yRotatioMatrix[0] = cos(yAngle);
	yRotatioMatrix[2] = -sin(yAngle);
	yRotatioMatrix[8] = sin(yAngle);
	yRotatioMatrix[10] = cos(yAngle);

	Matrix<float> translateMatrix = viewMatrix;
	translateMatrix[3] = -position[0];
	translateMatrix[7] = -position[1];
	translateMatrix[11] = -position[2];


	viewMatrix = translateMatrix * yRotatioMatrix * xRotatioMatrix;

	return viewMatrix;
}


void Camera::move() {

	float dx = 0.0f;
	float dy = 0.0f;
	float dz = 0.0f;

	float angleX = getYaw();
	float angleY = getPitch();

	speed = 0.02;

	// Camera movement (depend on angle)
	if (keyboardState.Shift == DOWN)
		speed *= 5.f;

	if (keyboardState.W == DOWN) {
		dx = (float)-sin(angleX / 180. * PI) * speed;
		dy = (float)tan(angleY / 180 * PI) * speed;
		dz = (float)-cos(angleX / 180 * PI) * speed;
	}
	if (keyboardState.S == DOWN) {
		dx = (float)sin(angleX / 180 * PI) * speed;
		dy = (float)-tan(angleY / 180 * PI) * speed;
		dz = (float)cos(angleX / 180 * PI) * speed;
	}
	if (keyboardState.D == DOWN) {
		dx = (float)-sin((angleX + 90) / 180 * PI) * speed;
		dz = (float)-cos((angleX + 90) / 180 * PI) * speed;
	}
	if (keyboardState.A == DOWN) {
		dx = (float)-sin((angleX - 90) / 180 * PI) * speed;
		dz = (float)-cos((angleX - 90) / 180 * PI) * speed;
	}

	// Apply move
	getPosition()[0] -= dx;
	getPosition()[1] -= dy;
	getPosition()[2] += dz;


	// Apply camera angle changing
	if (mouseState.left_button == DOWN && keyboardState.Shift == UP)
	{
		setPitch(getPitch() + mouseState.shiftY / 40.f);
		setYaw(getYaw() + mouseState.shiftX / 40.f);
		if (pitch < -89.9) setPitch(-89.9f);
		if (pitch > 89.9) setPitch(89.9f);
	}

	// Updating projection matrix
	Matrix<float>temp = createViewMatrix(pitch, yaw, position);
//	temp.transposed().print();
	projectionMatrix = perspectiveMatrix * temp;
	//projectionMatrix.transposed().print();
	// Matrix.multiplyMV(lightDirection, lightDirection.clone(), projectionMatrix);
}

float* Camera::getAngle() {
	return angle;
}

float* Camera::getPosition() {
	return position;
}

float Camera::getYaw() {
	return yaw;
}

void Camera::setYaw(float _yaw) {
	yaw = _yaw;
}

float Camera::getPitch() {
	return pitch;
}

void Camera::setPitch(float _pitch) {
	pitch = _pitch;
}

Matrix<float> Camera::getCameraMatrix() {
	move();
	return projectionMatrix;
}

