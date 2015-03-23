#include "behaviour\behaviourSystem.h"
#include <iostream>

BehaviourSystem::BehaviourSystem()
{
	_objMngrInt = nullptr;
}

BehaviourSystem::~BehaviourSystem()
{
}


bool BehaviourSystem::addBehaviour(SPtr_Behaviour behaviour)
{
	if(!behaviour) return false;
	_initializeList.push_back(behaviour);
	return true;
}


bool BehaviourSystem::addCustom(SPtr_Custom custom)
{
	if(!custom) return false;
	_customList.push_back(custom);
	return true;
}



void BehaviourSystem::clear()
{
	_customList.clear();
	_initializeList.clear();
	_updateList.clear();
	_fixedUpdateList.clear();
}


void BehaviourSystem::update(float t)
{
	// LOAD REQUESTED BEHAVIOURS
	for(int i = 0; i <_customList.size(); ++i)
	{
		if(_customList[i]->requestBehaviour())
		{
			loadBehaviour(_customList[i]);
		}
	}


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
			case EventType::LATE_UPDATE: _lateUpdateList.push_back(behaviour);		 break;
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

void BehaviourSystem::loadBehaviour(SPtr_Custom custom)
{
	if(_objMngrInt == nullptr) return;
	custom->behvrRequestMet(); // to say we have met request and set behaviour if possible
	if(!custom) return;

	// Get new behaviour if valid
	SPtr_Behaviour newBehaviour = CustomBehaviours::getBehaviour(custom->getBehvrName());
	if(!newBehaviour) return;

	// Destroy old behaviour if exists
	SPtr_Behaviour oldBehaviour = custom->getBehaviour();
	if(oldBehaviour != nullptr)
	{
		removeBehaviour(oldBehaviour);
	}

	

	// Find object by ID
	SPtr_GameObject obj;
	std::unordered_map<unsigned int, SPtr_GameObject>::const_iterator it; // iterator for searching map
	it = _objMngrInt->_gameObjects->find(custom->getObjectID());
	if(it == _objMngrInt->_gameObjects->end()) 
		return; // Failed! Iterator == end indicates key not found
	else
		obj = it->second;
	
	// Link object to behaviour system and custom object
	newBehaviour->linkToObject(obj);
	newBehaviour->linkToObjectMngrInt(*_objMngrInt);
	addBehaviour(newBehaviour);
	custom->setBehaviour(newBehaviour);
}


void BehaviourSystem::removeBehaviour(SPtr_Behaviour behaviour)
{
	for(int i = 0; i < _initializeList.size(); ++i)
	{
		if(behaviour == _initializeList[i])
		{
			_initializeList.erase(_initializeList.begin()+i);
			continue;
		}
	}
	for(int i = 0; i < _updateList.size(); ++i)
	{
		if(behaviour == _updateList[i])
		{
			_updateList.erase(_updateList.begin()+i);
			continue;
		}
	}
	for(int i = 0; i < _fixedUpdateList.size(); ++i)
	{
		if(behaviour == _fixedUpdateList[i])
		{
			_fixedUpdateList.erase(_fixedUpdateList.begin()+i);
			continue;
		}
	}
	for(int i = 0; i < _lateUpdateList.size(); ++i)
	{
		if(behaviour == _lateUpdateList[i])
		{
			_lateUpdateList.erase(_lateUpdateList.begin()+i);
			continue;
		}
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


void BehaviourSystem::lateUpdate(float t)
{
	// Call late update on any behaviours in the update list
	for(unsigned int i = 0; i < _lateUpdateList.size(); ++i)
	{
		_lateUpdateList[i]->lateUpdate(t);
	}

}