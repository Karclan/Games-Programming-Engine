#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "core\component.h"
#include "core\transform.h"

/*! \brief Physics Body for handling collisions


*/
#include <iostream>

class PhysicsBody : public Component
{
public:
	PhysicsBody(){};

	ComponentType::Type getType(); //!< Required implementation. Return type of component
	bool isOnePerObject(); //!< Required implementation. Return true if you can only have one of these per object
	
	void setTransform(SPtr_Transform transform) { _transform = transform; }//!< Requires transform


	void test() { std::cout << "PHysics me!\n"; }
	

private:
	SPtr_Transform _transform; //!< Pointer to transform
	

};

//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<PhysicsBody> SPtr_PhysBody;

#endif