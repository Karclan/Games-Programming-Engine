#include "behaviour\behaviourSystem.h"
#include <iostream>

BehaviourSystem::BehaviourSystem()
{
}

BehaviourSystem::~BehaviourSystem()
{
}


bool BehaviourSystem::addBehaviour(unsigned int ObjectID, SPtr_Behaviour behaviour)
{
	if(!behaviour) return false;

	// Find object by ID
	SPtr_GameObject obj = ObjectManager::getGameObject(ObjectID);
	if(!obj) return false; // Failed! Return false
	
	// Link object to script and add to behaviour system
	behaviour->linkToObject(obj);
	_initializeList.push_back(behaviour);
	return true;
}



void BehaviourSystem::clear()
{
	_initializeList.clear();
	_updateList.clear();
	_fixedUpdateList.clear();
}


void BehaviourSystem::update(float t)
{
	// INITIALIZE
	// Call initialize on any newly added behaviours
	while(_initializeList.size() > 0)
	{
		SPtr_Behaviour behaviour = _initializeList.back();
		_initializeList.pop_back();
		if(!behaviour->baseInitialize()) continue; //!< Checks for validity and calls initialize if ok
	
		// Cycle through requested events and add to relevant vectors
		for(int i = 0; i < behaviour->_requestedEvents.size(); ++i)
		{
			switch(behaviour->_requestedEvents[i])
			{
			case EventType::UPDATE: _updateList.push_back(behaviour);				 break;
			case EventType::FIXED_UPDATE: _fixedUpdateList.push_back(behaviour);	 break;
			}
		};

		behaviour->_requestedEvents.clear(); //! Clear all event listener requests
	}

	// UPDATE
	// Call update on any behaviours in the update list
	for(signed int i = _updateList.size()-1; i >= 0; i--)
	{
		if(!_updateList[i]->baseUpdate(t)) _updateList.erase(_updateList.begin()+i);
	}
}



void BehaviourSystem::fixedUpdate(float t)
{
	// Call fixed update on any behaviours in the update list
	for(unsigned int i = 0; i < _fixedUpdateList.size(); ++i)
	{
		_fixedUpdateList[i]->fixedUpdate(t);
	}

}