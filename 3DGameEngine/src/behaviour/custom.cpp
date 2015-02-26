#include "behaviour\custom.h"


Custom::Custom(unsigned int objectID) 
{
	_behvrName = ""; 
	_requestBehaviour = true;
	_objectID = objectID;
}

ComponentType::Type Custom::getType() 
{ 
	return ComponentType::CUSTOM;
}

bool Custom::isOnePerObject() 
{
	return true;
}


std::string Custom::getBehvrName()
{ 
	return _behvrName; 
}

void Custom::setBehaviour(std::string behvrName) 
{ 
	if(behvrName == _behvrName) return;
	_behvrName = behvrName;
	_requestBehaviour = true;
}

void Custom::setBehaviour(SPtr_Behaviour behaviour) 
{ 
	_behaviour = behaviour;
}

bool Custom::requestBehaviour() 
{
	return _requestBehaviour;
}

void Custom::behvrRequestMet()
{
	_requestBehaviour = false; 
}

SPtr_Behaviour Custom::getBehaviour() 
{ 
	return _behaviour;
}

unsigned int Custom::getObjectID()
{ 
	return _objectID; 
}