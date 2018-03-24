#ifndef BUTTN
#define BUTTN

#include "ActionManager.h"
#include "Rectangle.h"

class Button : public  Rectangle
{
private:
	bool pressed;
	Executable* action;
public:
	Button(float _posX, float _posY, float _sizeX, float _sizeY, char * texture);
	~Button();
	void setPressed(bool value);
	void setAction(Executable* action);

	void checkInteraction(float x, float y);

	bool isPressed();
};

#endif