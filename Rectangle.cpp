#include "stdafx.h"
#include "Rectangle.h"


Rectangle::Rectangle():
	Rectangle(0.0f, 0.0f, 1.0f, 1.0f, "")
{
}

Rectangle::Rectangle(float _posX, float _posY, float _sizeX, float _sizeY, char * _textureSource) :
	posX(_posX), posY(_posY), sizeX(_sizeX), sizeY(_sizeY)
{
	float x1 = _posX;
	float x2 = _posX + _sizeX;
	float y1 = _posY;
	float y2 = _posY - _sizeY;

	// Rectangle vertices positions
	vertices[0] = x1;  vertices[1] = y2;  vertices[2] = 0.f;  // skipping 2
	vertices[5] = x2;  vertices[6] = y2;  vertices[7] = 0.f;  // values
	vertices[10] = x2; vertices[11] = y1; vertices[12] = 0.f; // to left them
	vertices[15] = x1; vertices[16] = y1; vertices[17] = 0.f; // for texture

	// Rectangle vertices texture coordinates
	vertices[3] = 0.f;  vertices[4] = 0.f;
	vertices[8] = 1.f;  vertices[9] = 0.f;
	vertices[13] = 1.f; vertices[14] = 1.f;
	vertices[18] = 0.f; vertices[19] = 1.f;

	textureSource = _textureSource;

}

Rectangle::~Rectangle()
{
}

float * Rectangle::getVerticesData()
{
	return vertices;
}

int Rectangle::getVerticesDataLen()
{
	return vertexDataLen;
}

char * Rectangle::getTextureSource()
{
	return textureSource;
}

float Rectangle::getPosX()
{
	return posX;
}

float Rectangle::getPosY()
{
	return posY;
}

float Rectangle::getSizeX()
{
	return sizeX;
}

float Rectangle::getSizeY()
{
	return sizeY;
}
