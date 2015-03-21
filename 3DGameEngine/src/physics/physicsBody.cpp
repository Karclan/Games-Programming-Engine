#include "physics\physicsBody.h"

PhysicsBody::PhysicsBody()
{
	setDepFlag(ComponentType::TRANSFORM); // requires a transform

	setOptionalDepFlag(ComponentType::SPHERE_COL); // cache collider if it exists

	_mass = 1.0f; // important - mass of zero would give divide by zero error...
	_drag = 8.2f;
	_gravity = -36; // probably has to be stupid high coz drag high. Friction?
	_accel = glm::vec3();
	_velocity = glm::vec3();
}

ComponentType::Type PhysicsBody::getType()
{
	return ComponentType::PHY_BODY;		
}

bool PhysicsBody::isOnePerObject()
{
	return true;
}

void PhysicsBody::linkDependency(SPtr_Component component)
{
	// Switch through type and cast then cache the component
	switch(component->getType())
	{
	case ComponentType::TRANSFORM:
		_transform = std::static_pointer_cast<Transform>(component);
		break;
	}
}


void PhysicsBody::fixedUpdate(float t)
{
	_transform->translate(_velocity * t);

	_accel += glm::vec3(0, _gravity, 0);
	_velocity += (_accel - (_velocity * _drag)) * t;
	
	_accel = glm::vec3(); // reset to zero, the idea being you must add force every frame

	
}

void PhysicsBody::addForce(glm::vec3 force)
{
	_accel += force;
}
void PhysicsBody::addImpulse(glm::vec3 impulse)
{
	_velocity += impulse;
}