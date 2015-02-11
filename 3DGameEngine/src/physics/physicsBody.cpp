#include "physics\physicsBody.h"

PhysicsBody::PhysicsBody()
{
	setDepFlag(ComponentType::TRANSFORM); // requires a transform

	setOptionalDepFlag(ComponentType::SPHERE_COL); // cache collider if it exists
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


void PhysicsBody::resolve(Collision &collision)
{
	_transform->translate(collision.normal * -collision.penDepth);
}