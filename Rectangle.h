#ifndef RECTGL
#define RECTGL


class Rectangle
{
private:
	const float vertexDataLen = 20;
	float vertices[20];
	char* textureSource;

	float posX;
	float posY;
	float sizeX; 
	float sizeY;
public:
	Rectangle();
	Rectangle(
		float _posX, float _posY, 
		float _sizeX, float _sizeY,
		char* _textureSource = "");
	~Rectangle();

	float* getVerticesData();
	int getVerticesDataLen();
	char* getTextureSource();

	float getPosX();
	float getPosY();
	float getSizeX();
	float getSizeY();
};


#endif
