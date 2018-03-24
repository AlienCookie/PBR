#include "stdafx.h"
#include "ActiveObjectAction.h"


ObjectResetAction::ObjectResetAction(GLObject *object)
{
	objectToReset = object;
}


ObjectResetAction::~ObjectResetAction()
{
}

void ObjectResetAction::execute()
{
	objectToReset->setOnDraw(!objectToReset->isOnDraw());
	objectToReset->resetMatrices();
}
