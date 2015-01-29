#ifndef OBJECT_CREATOR_H
#define OBJECT_CREATOR_H

#include <unordered_map>
#include <list>

#include "core\gameObject.h"
#include "rendering\renderSystem.h"
#include "physics\physicsSystem.h"



/*! \brief Manages and stores all objects (Singleton)

	Originally this was not a singleton but has been changed so behaviours can use static functions
	to search for other objects. Depending on how scripting works if behaviours become scripts later
	this may become a non-singleton again. A pointer to the instance is still stored in Engine (originally
	this was simply a member variable instance on the stack).

	*Note that behaviours are not added to Game Objects through this interface. Scene Manager talks
	directly to behaviour system. This is because behaviours can reference game objects and are not
	components (to avoid circular references).

*/
class ObjectManager
{
public:
	static ObjectManager* startUp(RenderSystem &rendSys, PhysicsSystem &physicsSys);
	
	// Static interface fucntions for behaviours. Could also be used for sending messages between game objects
	static SPtr_GameObject getGameObject(unsigned int objectID); //!< Returns an object by unique ID number
	static SPtr_GameObject getGameObject(std::string objectName); //!< Searches for a game object by name and returns first one with that name or null if not found

	// Public functions, non-static so only called by Engine and SceneManager
	// At the moment we're saying you can only create and destroy objects in "Editor Mode" - you will need different functions for dynamic object instatiation (for scripts to interface with)
	unsigned int createGameObject(std::string name); //!< Create object and return its unique ID
	bool addComponent(unsigned int ObjectID, SPtr_Component component); //!< Find object by id and add component to it if exists. Return true on success, false on fail
	void destroyAll(); //!< Clear all objects

private:
	unsigned int _nextId; //!< Starts at 0 and assigned as unique ID when object created, then incremented by one, allowing 'uint's max limit' objects per scene. Reset to zero when all objects cleared from map
	std::unordered_map<unsigned int, SPtr_GameObject> _gameObjects; //!< All gameObjects, stored next to its unique ID

	// Sub systems Obj Manager should add any components they need on creation
	// e.g. rendering components sent to rendering system, rigidbodies sent to physics etc
	RenderSystem* _rendSys;
	PhysicsSystem* _physicsSys;


	// Private functions
	ObjectManager(); // private constructor prevents mutiple instances
	static ObjectManager* get(); // private so only this class can access the instance, others must interface through static functions
	
};

#endif