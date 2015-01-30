#ifndef OBJECT_FINDER_H
#define OBJECT_FINDER_H

#include <unordered_map>
#include <vector>

#include "core\gameObject.h"


/*! \brief Class for behaviours to use to find other game objects

	This class has been created to take a lot of the issues of having ObjectManager
	used for getting objects (circular reference problems, object manager being a singleton
	which is undesirable etc).

*/

class ObjectFinder
{
public:
	//friend class Behaviour; // Friend so only behaviour can access data
	ObjectFinder();
	~ObjectFinder(){};

	void setReferences(std::unordered_map<unsigned int, SPtr_GameObject> &gameObjects);

//private:
	const std::unordered_map<unsigned int, SPtr_GameObject>* _gameObjects; //!< Const Pointer to the map of game objects

};







#endif
