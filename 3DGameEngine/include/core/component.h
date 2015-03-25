#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory> // for shared pointers
#include "globals.h" // for bitmasks

//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
class Component; // fwd declaration so can define shared pointer
typedef std::shared_ptr<Component> SPtr_Component;

// Wrapping enums in namespaces makes the feel more OO, forcing ComponentType::CAMERA for example rather than just CAMERA
//! Different types of component
namespace ComponentType
{
	// IMPORTANT! - Bitmask is defined in globals as an uint, minimum bits for an uint in C++ standard is 16 (usually 32 on 
	// a modern computer though). If we make more than 16 types of component consider changing to an unsigned long.
	enum Type { TRANSFORM, MODL_REND, CAMERA, ROB_REND, PHY_BODY, LIGHT, SPHERE_COL, BOX_COL, ANIMATION, CUSTOM, TERRAIN_COL };


}

//! Different state the component can be in
namespace ComponentState
{
	enum State { ACTIVE, INACTIVE, DESTROYED };
}


/*! \brief Base class for all components

	A vector of components is stored in thr GameObject instance. Shared pointers
	to these components are then passed to the various sub-systems that need them.

*/
class Component
{
public:
	Component();
	~Component();

	virtual ComponentType::Type getType() = 0; //!< Pure virtual forces children to implement function returning whatever type the component is
	virtual bool isOnePerObject() = 0; //!< Pure virtual forces children to implement function specifying if more than one of these can be added to a game object
	
	ComponentState::State getState() { return _lifeCycleState; }

	virtual void linkDependency(SPtr_Component component){}; //!< Override to link needed dependencies, e.g. switch desired types and cache in a variable. Make sure the components have been requested in the dependencyFlags variable.
	virtual void init(){}; // override if any specific init logic is required (other than setting dynamic init variables)

	BITMASK getDepFlags();
	BITMASK getOptionalDepFlags();
	void destroy(); //!< Calling this will set a flag telling all subsystems to remove component

protected:
	void setDepFlag(ComponentType::Type type); //!< Set a flag in dependency flags
	bool getIfDepFlagSet(ComponentType::Type type); //!< Get if specified component type flag is set
	void setOptionalDepFlag(ComponentType::Type type); //!< Set a flag in dependency flags


private:
	ComponentState::State _lifeCycleState; //!< Current life cycle state, active, inactive or destroyed
	BITMASK _dependencyFlags; //!< Flag to specify which other components it needs a reference to (if any)
	BITMASK _optionalDepFlags; //!< Flags to specify optional dpendecies - ie if it has these components, we'd like to know about them
	


};



#endif