#include "stdafx.h"
#include "ActionManager.h"

MouseState mouseState;
KeyboardState keyboardState;

ActionManager::ActionManager(std::list<EditorObject *>_activeObject)
{
	activeObjects = _activeObject;

}

ActionManager::ActionManager()
{

}

void ActionManager::addItem(EditorObject * activeObject)
{
	activeObjects.push_back(activeObject);
}

ActionManager::~ActionManager()
{
}

void ActionManager::mouseHandle()
{
	if (activeObjects.empty())
		return;

	if (mouseState.left_button == DOWN && keyboardState.Shift == UP) {
		vec3f angles{ 
			(float) mouseState.shiftY / 60.f,
			(float) mouseState.shiftX / 60.f,
			0.f };
		for (auto activeObject = activeObjects.begin(); activeObject != activeObjects.end(); activeObject++)
			(*activeObject)->rotate(angles);
	}
	if (mouseState.right_button == DOWN && keyboardState.Shift == UP) {
		vec3f shift{
			(float)mouseState.shiftX / 500.f,
			(float)-mouseState.shiftY / 250.f,
			0.f };
		for (auto activeObject = activeObjects.begin(); activeObject != activeObjects.end(); activeObject++)
			(*activeObject)->translate(shift);
	}
	if (mouseState.right_button == DOWN && keyboardState.Shift == DOWN) {
		vec3f shift{
			(float)mouseState.shiftX / 500.f,
			0.f,
			(float)-mouseState.shiftY / 250.f };
		for (auto activeObject = activeObjects.begin(); activeObject != activeObjects.end(); activeObject++)
			(*activeObject)->translate(shift);
	}
	if (mouseState.midle_button == DOWN && keyboardState.Shift == DOWN) {
		vec3f factor{
			1.f + (float)mouseState.shiftX / 400.f,
			1.f + (float)mouseState.shiftY / 300.f,
			1.f + (float)mouseState.shiftY / 300.f, };
		for (auto activeObject = activeObjects.begin(); activeObject != activeObjects.end(); activeObject++)
			(*activeObject)->scale(factor);
	}
	if (mouseState.midle_button == DOWN && keyboardState.Shift == UP) {
		float fc = (float)mouseState.shiftX / 400.f + (float)mouseState.shiftY / 300.f;
		vec3f factor{
			1.f + fc,
			1.f + fc,
			1.f + fc };
		for (auto activeObject = activeObjects.begin(); activeObject != activeObjects.end(); activeObject++)
			(*activeObject)->scale(factor);
	}

	mouseState.shiftX = 0;
	mouseState.shiftY = 0;
}

void ActionManager::addActiveObject(EditorObject * _activeObject)
{
	activeObjects.push_back(_activeObject);
}

void ActionManager::addAllActiveObject(std::list<EditorObject *> _activeObjects)
{
	activeObjects.merge(_activeObjects);
}

void ActionManager::replaceAllActiveObject(std::list<EditorObject *> _activeObjects)
{
	activeObjects.clear();
	addAllActiveObject(_activeObjects);
}