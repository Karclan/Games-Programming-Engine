#ifndef BEHAVIOUR_SYSTEM_H
#define BEHAVIOUR_SYSTEM_H

#include <vector>
#include <memory>
#include <algorithm>

//#include "core\objectManager.h"
#include "behaviour\behaviour.h"
#include "behaviour\playerController.h" // this is temporary, as no real scripts yet it needs to know EVERY hard-coded behaviour
#include "behaviour\rotatingObject.h" // this is temporary, as no real scripts yet it needs to know EVERY hard-coded behaviour


// Temporary - needed to load behaviours from XML but in finished engine behaviours will be script files so can be loaded by file path
namespace BehaviourTypes
{
	enum Type { PLAYER_CON, ROT_OBJ, MAN_ROT };
}


/*! \brief Behaviour Sub-System

	System for managing and updating all behaviours. When true scripting is implemented
	this whole system may need to be rewritten but that's the advantage of separting
	different aspects of the engine into "sub systems" like this.

*/
class BehaviourSystem
{
public:
	BehaviourSystem();
	~BehaviourSystem();

	bool addBehaviour(SPtr_Behaviour behaviour);
	void clear();
	void update(float t); //!< Call update on all in update list and intialize on new behaviours. t = deltat time
	void fixedUpdate(float t); // t = fixed delta time

private:
	std::vector<SPtr_Behaviour> _initializeList; //!< List of behaviours to initialize
	std::vector<SPtr_Behaviour> _updateList; //!< List of behaviours that want update event
	std::vector<SPtr_Behaviour> _fixedUpdateList; //!< List of behaviours that want fixed update event

};







#endif