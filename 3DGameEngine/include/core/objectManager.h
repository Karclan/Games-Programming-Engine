#ifndef OBJECT_CREATOR_H
#define OBJECT_CREATOR_H

#include <unordered_map>
#include <list>

#include "tinyXML\tinystr.h"
#include "tinyXML\tinyxml.h"

#include "core\initTable.h"
#include "core\gameObject.h"
#include "core\objectFinder.h"
#include "behaviour\behaviour.h"
#include "behaviour\behaviourSystem.h"
#include "rendering\renderSystem.h"
#include "physics\physicsSystem.h"



/*! \brief Manages and stores all objects

	Manages creation and deletion of game objects, components and behaviours. Note that currently
	the creation functions are designed to only be called before the game starts (e.g. by XML loader
	or within the Editor). No dynamic runtime creation yet because it auto-adds to init table, meaning
	if you created something during game play it would still be there when you quit to the editor!

*/
class ObjectManager
{
public:
	ObjectManager();
	void startUp(RenderSystem &rendSys, PhysicsSystem &physicsSys, BehaviourSystem &behvrSys);
	
	unsigned int createGameObject(std::string name); //!< Create object and return its unique ID
	bool addComponent(unsigned int objectID, ComponentType::Type type); //!< Find object by id and add component to it if exists. Return true on success, false on fail
	bool addBehaviour(unsigned int objectID, SPtr_Behaviour behaviour); 
	SPtr_GameObject getGameObject(unsigned int objectID); //!< Returns an object by unique ID number
	SPtr_GameObject getGameObject(std::string objectName); //!< Searches for a game object by name and returns first one with that name or null if not found	
	bool addComponentsFromXML(unsigned int objectID, TiXmlElement* goElmnt); //!< Add all components from a tiny xml elemnet (tagged GO)
	bool addBehavioursFromXML(unsigned int objectID, TiXmlElement* goElmnt); //!< Add all behaviours from a tiny xml elemnet (tagged GO)
	void linkComponents(unsigned int goID); //!< Ensures all components in an object are linked. If one requires a component that isn't there is creates and adds it
	void destroyAll(); //!< Clear all objects
	
	// Init table management
	void clearInitTable() { _initTable.clear(); }
	InitTable* getInitTable() { return &_initTable; }


private:
	unsigned int _nextId; //!< Starts at 0 and assigned as unique ID when object created, then incremented by one, allowing 'uint's max limit' objects per scene. Reset to zero when all objects cleared from map
	std::unordered_map<unsigned int, SPtr_GameObject> _gameObjects; //!< All gameObjects, stored next to its unique ID
	ObjectFinder _objectFinder;
	InitTable _initTable;

	// Sub systems Obj Manager should add any components they need on creation
	// e.g. rendering components sent to rendering system, rigidbodies sent to physics etc
	RenderSystem* _rendSys;
	PhysicsSystem* _physicsSys;
	BehaviourSystem* _behvrSys;

	
	// This is the funtion you'll want to edit when you make new types of component
	bool addUnlinkedComponent(unsigned int objectID, ComponentType::Type type); //!< Add a component to an object, but don't link its dependencies. Only use when adding lots of compnents and linking immediately after.
	bool addUnlinkedComponent(unsigned int objectID, ComponentType::Type type, TiXmlElement* compElmnt); //!< Overload allows you to pass XML object to read init values from XML
	
};

#endif