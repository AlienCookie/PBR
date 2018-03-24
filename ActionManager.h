#ifndef ACT_MEN
#define ACT_MEN

#include "EditorObject.h"
#include <list>

enum ButtonState { UP, DOWN };

struct MouseState {
	double x;
	double y;
	double shiftX;
	double shiftY;
	double clickPositionX;
	double clickPositionY;
	ButtonState left_button = UP;
	ButtonState right_button = UP;
	ButtonState midle_button = UP;
	int windowWidth;
	int windowHeight;
};

struct KeyboardState {
	ButtonState W = UP;
	ButtonState A = UP;
	ButtonState D = UP;
	ButtonState S = UP;
	ButtonState Shift = UP;
};


extern MouseState mouseState;
extern KeyboardState keyboardState;

class Executable{
public:
	virtual void execute() = 0;
};


class ActionManager
{
private:
	// Object that should respond to actions
	std::list<EditorObject *> activeObjects;
public:
	ActionManager(std::list<EditorObject *>);
	ActionManager();
	void addItem(EditorObject * activeObject);
	~ActionManager();
	void mouseHandle();
	//void keysHandler();

	void addActiveObject(EditorObject* _activeObject);
	void addAllActiveObject(std::list<EditorObject*> _activeObjects);
	void replaceAllActiveObject(std::list<EditorObject*> _activeObjects);
};

#endif

