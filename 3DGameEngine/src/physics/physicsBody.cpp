#include "physics\physicsBody.h"

PhysicsBody::PhysicsBody()
{
	setDepFlag(ComponentType::TRANSFORM); // requires a transform

	setOptionalDepFlag(ComponentType::SPHERE_COL); // cache collider if it exists

	_awake = true;

	_mass = 1.0f; // important - mass of zero would give divide by zero error...
	_drag = 3.0f;
	_gravity = -34; // probably has to be stupid high coz drag high. Friction?
	_accel = glm::vec3(0, 0.001f, 0); // to ensure if it's in the air on startup it doesn't go to sleep because velocity is zero
	_velocity = glm::vec3();
}

PhysicsBody::~PhysicsBody()
{
	std::cout << "PhysicsBody DESTORYED!\n";
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
	if(_accel == glm::vec3())
	{
		float sumV = (std::abs(_velocity.x) + std::abs(_velocity.y) + std::abs(_velocity.z)) * 0.33333f;
		if(sumV < 0.01f)
		{
			_velocity = glm::vec3();
			_awake = false;
			return;
		}
	}

	_transform->translate(_velocity * t);

	_accel += glm::vec3(0, _gravity, 0);
	_velocity += (_accel - (_velocity * _drag)) * t;
	
	_accel = glm::vec3(); // reset to zero, the idea being you must add force every frame

	
}

void PhysicsBody::addForce(glm::vec3 force)
{
	_accel += force;
	_awake = true;
}
void PhysicsBody::addImpulse(glm::vec3 impulse)
{
	_velocity += impulse;
	_awake = true;
}

void PhysicsBody::setVelocity(glm::vec3 v) 
{ 
	_velocity = v;
	_awake = true;
}