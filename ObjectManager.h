#ifndef OBJ_MENG
#define OBJ_MENG

#include <list>
#include "EditorObject.h"

class ObjectManager
{
public:
	static void loadFromOBJ(char* source, std::list<EditorObject*> *outList, int lightningType);
};

#endif