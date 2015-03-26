#include "behaviour\objectMngrInterface.h"


ObjectMngrInterface::ObjectMngrInterface()
{
	_gameObjects = nullptr;
	_dynInitdObjs = nullptr;
}

void ObjectMngrInterface::setReferences(std::unordered_map<unsigned int, SPtr_GameObject> &gameObjects, std::set<SPtr_GameObject> &dynInitObjs)
{
	_gameObjects = &gameObjects;
	_dynInitdObjs = &dynInitObjs;
}
