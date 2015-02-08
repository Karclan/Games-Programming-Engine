#include "physics\collider.h"

Collider::Collider()
{
	setDepFlag(ComponentType::TRANSFORM); // requires a transform
}

void Collider::linkDependency(SPtr_Component component)
{
	// Switch through type and cast then cache the component
	switch(component->getType())
	{
	case ComponentType::TRANSFORM:
		_transform = std::static_pointer_cast<Transform>(component);
		break;
	}
}



// SPHERE COLLIDER!-------------------------------------------------
ComponentType::Type SphereCollider::getType()
{
	return ComponentType::SPHERE_COL;
}

void SphereCollider::test()
{
	std::cout << "Hello! I'm a sphere!\n";
}