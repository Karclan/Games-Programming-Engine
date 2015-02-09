#include "physics\collider.h"

Collider::Collider()
{
	_transform = nullptr;
	_physicsBody = nullptr;

	setDepFlag(ComponentType::TRANSFORM); // requires a transform

	setOptionalDepFlag(ComponentType::PHY_BODY); // cache physics body if it exists
}

void Collider::linkDependency(SPtr_Component component)
{
	// Switch through type and cast then cache the component
	switch(component->getType())
	{
	case ComponentType::TRANSFORM:
		_transform = std::static_pointer_cast<Transform>(component);
		break;

	case ComponentType::PHY_BODY:
		_physicsBody = std::static_pointer_cast<PhysicsBody>(component);

		break;
	}
}





//#################################################################
//------------------- SPHERE COLLIDER!-----------------------------
//#################################################################
SphereCollider::SphereCollider()
{
	_radius = 0.5f;
	_offset = glm::vec3(0, 0, 0);
}

ComponentType::Type SphereCollider::getType()
{
	return ComponentType::SPHERE_COL;
}


// Wonderful collision logic!
bool SphereCollider::collides(SPtr_Collider other)
{
	switch(other->getType())
	{
	case ComponentType::SPHERE_COL:
		{
			SPtr_SphereCol otherS = std::static_pointer_cast<SphereCollider>(other);
			glm::vec3 aToB = otherS->getCentre() - getCentre();
			float squareDist = glm::dot(aToB, aToB); // i think this would give you x*x + y*y + z*z !
			float radSum = _radius + otherS->getRadius();
			float squareRadSum = radSum * radSum;


			return squareDist < squareRadSum; // they're colliding if the distance is less than the sum of the radii
		}
	}


	return false;
}

float SphereCollider::getRadius()
{
	return _radius;
}

glm::vec3 SphereCollider::getCentre()
{
	return _transform->getPosition() + _offset;
}






//#################################################################
//---------------------- BOX COLLIDER!-----------------------------
//#################################################################
BoxCollider::BoxCollider()
{
	_extents = glm::vec3(1, 1, 1);
	_offset = glm::vec3(0, 0, 0);
}

ComponentType::Type BoxCollider::getType()
{
	return ComponentType::BOX_COL;
}


// Wonderful collision logic!
bool BoxCollider::collides(SPtr_Collider other)
{
	switch(other->getType())
	{
	case ComponentType::BOX_COL:
		{
			SPtr_BoxCol otherB = std::static_pointer_cast<BoxCollider>(other);
			
			// TO DO - Logic!
			
			return false;
		}
	}


	return false;
}

glm::vec3 BoxCollider::getExtents()
{
	return _extents;
}

glm::vec3 BoxCollider::getCentre()
{
	return _transform->getPosition() + _offset;
}
