#include "core\objectFinder.h"


ObjectFinder::ObjectFinder()
{
	_gameObjects = nullptr;
}

void ObjectFinder::setReferences(std::unordered_map<unsigned int, SPtr_GameObject> &gameObjects)
{
	_gameObjects = &gameObjects;
}
