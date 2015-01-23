#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory> // for shared pointers

// Wrapping enums in namespaces makes the feel more OO, forcing ComponentType::CAMERA for example rather than just CAMERA
//! Different types of component
namespace ComponentType
{
	enum Type { TRANSFORM, MODL_REND, CAMERA, ROB_REND, PHY_BODY, LIGHT };
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
	void destroy(); //!< Calling this wil

private:
	ComponentState::State _lifeCycleState; //!< Current life cycle state, active, inactive or destroyed

};

//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<Component> SPtr_Component;

#endif