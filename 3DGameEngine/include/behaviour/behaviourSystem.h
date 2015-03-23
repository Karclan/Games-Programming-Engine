#ifndef BEHAVIOUR_SYSTEM_H
#define BEHAVIOUR_SYSTEM_H

#include <vector>
#include <memory>
#include <algorithm>

#include "behaviour\custom.h"
#include "behaviour\behaviour.h"
#include "custom\customBehaviours.h"
#include "behaviour\objectMngrInterface.h"


/*! \brief Behaviour Sub-System

	System for managing and updating all behaviours.

*/
class BehaviourSystem
{
public:
	BehaviourSystem();
	~BehaviourSystem();

	void setObjMngrInt(ObjectMngrInterface &objMngrInt) { _objMngrInt = &objMngrInt; }
	bool addCustom(SPtr_Custom custom);
	void clear();
	void update(float t); //!< Call update on all in update list and intialize on new behaviours. t = deltat time
	void fixedUpdate(float t); // t = fixed delta time
	void lateUpdate(float t); // t = delta time

	
private:
	void loadBehaviour(SPtr_Custom custom); //!< Load behaviour requested by custom components
	void removeBehaviour(SPtr_Behaviour behaviour); //!< Remove behaviour from the system
	bool addBehaviour(SPtr_Behaviour behaviour); //!< Add new behaviour based on Custom component

	ObjectMngrInterface* _objMngrInt; //!< Pointer to the object finder
	std::vector<SPtr_Custom> _customList; //!< List of custom components
	std::vector<SPtr_Behaviour> _initializeList; //!< List of behaviours to initialize
	std::vector<SPtr_Behaviour> _updateList; //!< List of behaviours that want update event
	std::vector<SPtr_Behaviour> _fixedUpdateList; //!< List of behaviours that want fixed update event
	std::vector<SPtr_Behaviour> _lateUpdateList; //!< List of behaviours that want late update event


	// Object Manager interface - needs access to these data structures, can't go through obj manager though because circular references
	const std::unordered_map<unsigned int, SPtr_GameObject>* _gameObjects; //!< Const Pointer to the map of game objects
	std::set<SPtr_GameObject>* _dynInitdObjs; //!< Pointer to list of dynamically created objects that are waiting to be put into the system


};







#endif