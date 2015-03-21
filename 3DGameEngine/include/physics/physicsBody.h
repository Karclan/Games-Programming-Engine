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

	void fixedUpdate(float t); //!< Fixed update to handle integration etc
	
	void addForce(glm::vec3 force);  //!< Add force (accel)
	void addImpulse(glm::vec3 impulse); //!< Add impulse (instant velocity);

	glm::vec3 getVelocity() { return _velocity; }
	void setVelocity(glm::vec3 v) { _velocity = v; }
	SPtr_Transform getTransform() { return _transform; }
	float getMass() { return _mass; } // when make set mass, ensure to check for <= 0 so not negative or zero

private:
	SPtr_Transform _transform; //!< Pointer to transform

	float _mass;
	float _drag;
	float _gravity;
	glm::vec3 _accel;
	glm::vec3 _velocity;

};

//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<PhysicsBody> SPtr_PhysBody;


// Include after definition of physicsbody class because circular reference
#include "physics\collision.h"


#endif