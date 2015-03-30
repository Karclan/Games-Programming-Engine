#ifndef OBJECT_FINDER_H
#define OBJECT_FINDER_H

#include <unordered_map>
#include <vector>
#include <set>

#include "core\gameObject.h"


/*! \brief Class for behaviours to use to find other game objects

	This class has been created to take a lot of the issues of having ObjectManager
	used for getting objects (circular reference problems, object manager being a singleton
	which is undesirable etc).

*/

class ObjectMngrInterface
{
public:
	//friend class Behaviour; // Friend so only behaviour can access data
	ObjectMngrInterface();
	~ObjectMngrInterface(){};

	void setReferences(std::unordered_map<unsigned int, SPtr_GameObject> &gameObjects, std::set<SPtr_GameObject> &dynInitObjs);
	


//private:
	const std::unordered_map<unsigned int, SPtr_GameObject>* _gameObjects; //!< Const Pointer to the map of game objects
	std::set<SPtr_GameObject>* _dynInitdObjs; //!< Pointer to list of dynamically created objects that are waiting to be put into the system
	bool _loadLevel;
	std::string _levelPath;

};







#endif
