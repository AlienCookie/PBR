#ifndef ACT_OBJ_ACTION_
#define ACT_OBJ_ACTION_

#include "ActionManager.h"
#include "GLObject.h"

class ObjectResetAction : public Executable
{
private:
	GLObject *objectToReset;
public:
	ObjectResetAction(GLObject *object);
	~ObjectResetAction();
	void execute();
};

#endif