#include "behaviour\behaviour.h"
#include <iostream>

Behaviour::Behaviour()
{
	_initialized = false;
	//std::cout << "Behaviour Created!\n";
}

Behaviour::~Behaviour()
{
	//std::cout << "Behaviour Destroyed!\n";
}

void Behaviour::linkToObject(SPtr_GameObject gameObject)
{
	_gameObject = gameObject;
	_active = _gameObject->isActive();
}

void Behaviour::linkToObjectMngrInt(ObjectMngrInterface &objMngrInt)
{
	_objMngrInt = &objMngrInt;
}


void Behaviour::addEventListener(EventType type)
{
	_requestedEvents.push_back(type);
}


bool Behaviour::baseInitialize()
{
	if(!_gameObject) return false;
	initialize();
	_active = _gameObject->isActive();
	if(_active) onActive();
	return true;
}

void Behaviour::reset()
{
	initialize();
	_requestedEvents.clear();
}

void Behaviour::baseUpdate(float t)
{
	if(_active != _gameObject->isActive())
	{
		_active = _gameObject->isActive();
		if(_active) onActive();
	}
	update(t);
}

SPtr_Component Behaviour::getComponent(ComponentType::Type type)
{
	if(!_gameObject) return SPtr_Component();
	return _gameObject->getComponent(type);
}

int Behaviour::getGameObjectId()
{
	return _gameObject->getId();
}


SPtr_GameObject Behaviour::findGameObject(unsigned int objectID)
{
	// Find object by ID
	std::unordered_map<unsigned int, SPtr_GameObject>::const_iterator it; // iterator for searching map
	it = _objMngrInt->_gameObjects->find(objectID);
	if(it == _objMngrInt->_gameObjects->end()) 
		return SPtr_GameObject(nullptr); // Failed! Iterator == end indicates key not found
	else
		return it->second;

}


SPtr_GameObject Behaviour::findGameObject(std::string objectName)
{
	// Find object by ID
	std::unordered_map<unsigned int, SPtr_GameObject>::const_iterator it; // iterator for searching map

	if(_objMngrInt == nullptr) std::cout << "Object Finder is null\n";
	for(it = _objMngrInt->_gameObjects->begin(); it != _objMngrInt->_gameObjects->end(); ++it)
	{
		if(it->second->getName() == objectName) return it->second;
	}

	return SPtr_GameObject(nullptr); // Failed!
}


void Behaviour::addNewGameObject(SPtr_GameObject newObject)
{
	_objMngrInt->_dynInitdObjs->insert(newObject);
}

void Behaviour::getCollisions(SPtr_PhysBody myPhysBody, std::vector<SPtr_GameObject> &collidingObjs)
{
	const std::map<unsigned int, glm::vec3>* collisions = myPhysBody->getCollisions();
	std::map<unsigned int, glm::vec3>::const_iterator it;
	for(it = collisions->begin(); it != collisions->end(); it++)
	{
		collidingObjs.push_back(findGameObject(it->first));
	}
}

void Behaviour::setActive(bool active)
{
	_gameObject->setActive(active);
}

bool Behaviour::isActive()
{
	return _active;
}


void Behaviour::loadScene(std::string scene)
{
	_objMngrInt->_levelPath = scene;
	_objMngrInt->_loadLevel = true;
}