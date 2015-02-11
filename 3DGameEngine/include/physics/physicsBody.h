#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include <vector>

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "core\component.h"
#include "core\transform.h"

// FWD Declarations
class Collision;

/*! \brief Physics Body for handling collisions


*/
#include <iostream>

class PhysicsBody : public Component
{
public:
	PhysicsBody();

	ComponentType::Type getType(); //!< Required implementation. Return type of component
	bool isOnePerObject(); //!< Required implementation. Return true if you can only have one of these per object
	void linkDependency(SPtr_Component component); //!< Override to link needed dependencies, e.g. switch desired types and cache in a variable. Make sure the components have been requested in the dependencyFlags variable.

	void resolve(Collision &collision); //!< Given a collision info, resolve based on it

private:
	SPtr_Transform _transform; //!< Pointer to transform

};

//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<PhysicsBody> SPtr_PhysBody;


// Include after definition of physicsbody class because circular reference
#include "physics\collision.h"


#endif