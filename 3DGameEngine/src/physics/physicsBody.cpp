#include "physics\physicsBody.h"

PhysicsBody::PhysicsBody()
{
	setDepFlag(ComponentType::TRANSFORM); // requires a transform

	setOptionalDepFlag(ComponentType::SPHERE_COL); // cache collider if it exists

	wakeUp();

	_mass = 1.0f; // important - mass of zero would give divide by zero error...
	_drag = 1.2f;
	_gravity = -34; // probably has to be stupid high coz drag high. Friction?
	_accel = glm::vec3(0, 0.01f, 0); // to ensure if it's in the air on startup it doesn't go to sleep because velocity is zero
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
	// If no accel, go to sleep
	if(_accel == glm::vec3())
	{
		float sumV = (std::abs(_velocity.x) + std::abs(_velocity.y) + std::abs(_velocity.z)) * 0.33333f;
		if(sumV < 0.01f)
		{
			_sleepTimer += t;

			if(_sleepTimer > 0.5f)
			{
				_velocity = glm::vec3();
				_awake = false;
				return;
			}
		}
	}
	else
	{
		_sleepTimer = 0;
	}

	// Integration
	_transform->translate(_velocity * t);

	_accel += glm::vec3(0, _gravity, 0);
	_velocity += (_accel - (_velocity * _drag)) * t;
	
	_accel = glm::vec3(); // reset to zero, the idea being you must add force every frame


	// Grounding
	if(_ungroundLatency > 0)
		_ungroundLatency--;
	else
		_isGrounded = false;

	// Clear collision infos
	_collisions.clear();
	_triggers.clear();
	
	
}

void PhysicsBody::addForce(glm::vec3 force)
{
	_accel += force;
	wakeUp();
}
void PhysicsBody::addImpulse(glm::vec3 impulse)
{
	_velocity += impulse;
	wakeUp();
}

void PhysicsBody::setVelocity(glm::vec3 v) 
{ 
	_velocity = v;
	wakeUp();
}

void PhysicsBody::wakeUp()
{
	_awake = true;
	_sleepTimer = 0;
}

void PhysicsBody::addCollisionHit(unsigned int objId, glm::vec3 normal)
{
	if(normal.y > 0.5f)
	{
		_isGrounded = true;
		_ungroundLatency = 5;
	}
	_collisions.emplace(objId, normal);
}

void PhysicsBody::addTriggerHit(unsigned int objId)
{
	_triggers.push_back(objId);
}

bool PhysicsBody::hasCollided()
{
	return _collisions.size();
}

bool PhysicsBody::hasTriggered()
{
	return _triggers.size();
}