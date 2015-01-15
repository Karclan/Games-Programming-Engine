#include "behaviour\behaviour.h"
#include <iostream>

Behaviour::Behaviour()
{
	_initialized = false;
	std::cout << "Behaviour Created!\n";
}

Behaviour::~Behaviour()
{
	std::cout << "Behaviour Destroyed!\n";
}

void Behaviour::linkToObject(SPtr_GameObject gameObject)
{
	_gameObject = gameObject;
}


void Behaviour::addEventListener(EventType type)
{
	_requestedEvents.push_back(type);
}


bool Behaviour::baseInitialize()
{
	if(!_gameObject) return false;
	initialize();
	return true;
}

void Behaviour::reset()
{
	initialize();
	_requestedEvents.clear();
}

bool Behaviour::baseUpdate(float t)
{
	update(t);
	return true;
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