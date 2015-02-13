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
bool SphereCollider::collides(SPtr_Collider other, Collision &collInfo)
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

	case ComponentType::BOX_COL:
		{

			SPtr_BoxCol otherB = std::static_pointer_cast<BoxCollider>(other);


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
bool BoxCollider::collides(SPtr_Collider other, Collision &collInfo)
{
	switch(other->getType())
	{
	case ComponentType::BOX_COL:
		{
			SPtr_BoxCol otherB = std::static_pointer_cast<BoxCollider>(other);
			
			// Get verts in world space
			glm::vec3 myVerts[8];
			glm::vec3 otherVerts[8];
			getWorldVerts(myVerts);
			otherB->getWorldVerts(otherVerts);

			// Work out all 15 axes we need to check - if it can be done neatly, would be better to calc as we go to avoid calcs not needed
			// Also might be faster to transform unit fwd, right, up by matrix (omitting scale) assuming normalize uses sqrt
			glm::vec3 axes[15];
			axes[0] = myVerts[1] - myVerts[0]; // axis myRight
			axes[1] = myVerts[0] - myVerts[2]; // axis myUp
			axes[2] = myVerts[0] - myVerts[4]; // axis myBack (Z+, towards us)
			axes[3] = otherVerts[1] - otherVerts[0]; // axis otherRight
			axes[4] = otherVerts[0] - otherVerts[2]; // axis otherUp
			axes[5] = otherVerts[0] - otherVerts[4]; // axis otherBack (Z+, towards us)

			axes[6] = glm::cross(axes[0], axes[3]);
			axes[7] = glm::cross(axes[0], axes[4]);
			axes[8] = glm::cross(axes[0], axes[5]);

			axes[9] = glm::cross(axes[1], axes[3]);
			axes[10] = glm::cross(axes[1], axes[4]);
			axes[11] = glm::cross(axes[1], axes[5]);

			axes[12] = glm::cross(axes[2], axes[3]);
			axes[13] = glm::cross(axes[2], axes[4]);
			axes[14] = glm::cross(axes[2], axes[5]);

			
			// Now check each axis
			for(int a = 0; a < 15; ++a)
			{
				if(axes[a] == glm::vec3()) continue; // can happen if cross 2 vectors on same direction

				// Normalize axis, potentially slowest part of this (optimize later)
				axes[a] = glm::normalize(axes[a]);

				// Floats to store min and max distances across axes of each vert, init to dist on axes of vert 0 (see loop for details)
				float myMin = glm::dot(myVerts[0], axes[a]);
				float myMax = myMin;
				float otherMin = glm::dot(otherVerts[0], axes[a]);
				float otherMax = otherMin;

				// For every other vertex
				for(int v = 1; v < 8; ++v)
				{
					// Note that the dot product gives you distance from origin of vert projected on axis
					float myProjDist = glm::dot(myVerts[v], axes[a]);
					float otherProjDist = glm::dot(otherVerts[v], axes[a]); 
					myMin = std::min(myProjDist, myMin);
					myMax = std::max(myProjDist, myMax);
					otherMin = std::min(otherProjDist, otherMin);
					otherMax = std::max(otherProjDist, otherMax);
				}

				// If the biggest min is greater than the smallest max then they don't over lap so no collision				
				if(std::max(myMin, otherMin) > std::min(myMax, otherMax)) return false;


				float overlap = myMax - otherMin;
				float invOverlap = otherMax - myMin;

				if(overlap < invOverlap) // then I am on "left" so collision normal positive
				{
					if(a == 0 || std::abs(overlap) < collInfo.penDepth)
					{
						collInfo.normal = axes[a];
						collInfo.penDepth = overlap;
					}
				}
				else // then I am on "right" so collision normal negative
				{
					if(a == 0 || std::abs(invOverlap) < collInfo.penDepth)
					{
						collInfo.normal = -axes[a];
						collInfo.penDepth = invOverlap;
					}
				}
			}

			
			return true; // couldn't find axis of separation, they collide!
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

glm::mat4 BoxCollider::getTransformMatrix()
{
	return _transform->getMatrix() * glm::translate(_offset) * glm::scale(_extents);

}


void BoxCollider::getWorldVerts(glm::vec3 verts[])
{
	glm::mat4 transMat = getTransformMatrix();

	
	// near face
	verts[0] = glm::vec3(transMat * glm::vec4(-0.5f, 0.5f, 0.5f, 1.0)); // tl
	verts[1] = glm::vec3(transMat * glm::vec4(0.5f, 0.5f, 0.5f, 1.0)); // tr
	verts[2] = glm::vec3(transMat * glm::vec4(-0.5f, -0.5f, 0.5f, 1.0)); // bl
	verts[3] = glm::vec3(transMat * glm::vec4(0.5f, -0.5f, 0.5f, 1.0)); // br
	
	// far face
	verts[4] = glm::vec3(transMat * glm::vec4(0.5f, 0.5f, -0.5f, 1.0)); // tl
	verts[5] = glm::vec3(transMat * glm::vec4(-0.5f, 0.5f, -0.5f, 1.0)); // tr
	verts[6] = glm::vec3(transMat * glm::vec4(0.5f, -0.5f, -0.5f, 1.0)); // bl
	verts[7] = glm::vec3(transMat * glm::vec4(-0.5f, -0.5f, -0.5f, 1.0)); // br
	
}

void BoxCollider::test()
{
	// Debug
	glm::vec3 verts[8];
	getWorldVerts(verts);

	std::cout << "WORLD VERTS:\n";
	for(int i = 0; i < 8; ++i)
	{
		std::cout << verts[i].x << ", ";
		std::cout << verts[i].y << ", ";
		std::cout << verts[i].z << "\n";
	}
	std::cout << "\n\n";

}