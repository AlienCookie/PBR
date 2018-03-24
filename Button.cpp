#include "stdafx.h"
#include "Button.h"



Button::Button(float _posX, float _posY, float _sizeX, float _sizeY, char * texture) :
	Rectangle(_posX, _posY, _sizeX, _sizeY, texture)
{
	action = NULL;
	pressed = false;
}

void Button::setAction(Executable * _action)
{
	action = _action;
}

void Button::checkInteraction(float x, float y)
{
	if (x > getPosX() && x < (getPosX() + getSizeX())) {
		if (y < getPosY() && y > (getPosY() - getSizeY()))
		{
			pressed = !pressed;
			if(action != NULL)
				action->execute();
		}
	}
}

Button::~Button(){}

void Button::setPressed(bool value)
{
	pressed = value;
}

bool Button::isPressed()
{
	return pressed;
}

