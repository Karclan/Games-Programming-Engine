#ifndef BEHAVIOUR_SYSTEM_H
#define BEHAVIOUR_SYSTEM_H

#include <vector>
#include <memory>
#include <algorithm>

#include "behaviour\custom.h"
#include "behaviour\behaviour.h"
#include "custom\customBehaviours.h"
#include "core\objectFinder.h"


/*! \brief Behaviour Sub-System

	System for managing and updating all behaviours.

*/
class BehaviourSystem
{
public:
	BehaviourSystem();
	~BehaviourSystem();

	void setObjectFinder(ObjectFinder &objectFinder) { _objFinder = &objectFinder; }
	bool addCustom(SPtr_Custom custom);
	void clear();
	void update(float t); //!< Call update on all in update list and intialize on new behaviours. t = deltat time
	void fixedUpdate(float t); // t = fixed delta time
	void lateUpdate(float t); // t = delta time

private:
	void loadBehaviour(SPtr_Custom custom); //!< Load behaviour requested by custom components
	void removeBehaviour(SPtr_Behaviour behaviour); //!< Remove behaviour from the system
	bool addBehaviour(SPtr_Behaviour behaviour); //!< Add new behaviour based on Custom component

	ObjectFinder* _objFinder; //!< Pointer to the object finder
	std::vector<SPtr_Custom> _customList; //!< List of custom components
	std::vector<SPtr_Behaviour> _initializeList; //!< List of behaviours to initialize
	std::vector<SPtr_Behaviour> _updateList; //!< List of behaviours that want update event
	std::vector<SPtr_Behaviour> _fixedUpdateList; //!< List of behaviours that want fixed update event
	std::vector<SPtr_Behaviour> _lateUpdateList; //!< List of behaviours that want late update event
};







#endif