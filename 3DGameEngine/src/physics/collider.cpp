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

void SphereCollider::calculateBounds()
{
	_bounds.min = getCentre() - glm::vec3(_radius, _radius, _radius);
	_bounds.max = getCentre() + glm::vec3(_radius, _radius, _radius);
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


			if(squareDist < squareRadSum)
			{
				float distMag = std::sqrt(squareDist);
				collInfo.bodyA = _physicsBody;
				collInfo.bodyB = other->getPhysicsBody();
				collInfo.penDepth = radSum - distMag;
				collInfo.normal = aToB / distMag;
				return true;
			}
			return false;
		}

	case ComponentType::BOX_COL:
		{

			SPtr_BoxCol otherB = std::static_pointer_cast<BoxCollider>(other);

			// Get other's verts in world space
			const glm::vec3* otherVerts = otherB->getWorldVerts();

			// Work out all 15 axes we need to check - if it can be done neatly, would be better to calc as we go to avoid calcs not needed
			// Also might be faster to transform unit fwd, right, up by matrix (omitting scale) assuming normalize uses sqrt
			glm::vec3 axes[3];
			axes[0] = glm::normalize(otherVerts[1] - otherVerts[0]); // axis otherRight
			axes[1] = glm::normalize(otherVerts[0] - otherVerts[2]); // axis otherUp
			axes[2] = glm::normalize(otherVerts[0] - otherVerts[4]); // axis otherBack (Z+, towards us)

			// Distance along axis for box
			glm::vec2 boxMinMax[3];
			boxMinMax[0] = glm::vec2(glm::dot(otherVerts[0], axes[0]), glm::dot(otherVerts[1], axes[0])); // top left and top right along relative X axis
			boxMinMax[1] = glm::vec2(glm::dot(otherVerts[2], axes[1]), glm::dot(otherVerts[0], axes[1])); // bottom left and top left along relative Y axis
			boxMinMax[2] = glm::vec2(glm::dot(otherVerts[4], axes[2]), glm::dot(otherVerts[0], axes[2])); // far top left and near top left along relative Z axis

			// Distance along axis for sphere centre point
			float sphCentreDist[3];
			sphCentreDist[0] = glm::dot(getCentre(), axes[0]);
			sphCentreDist[1] = glm::dot(getCentre(), axes[1]);
			sphCentreDist[2] = glm::dot(getCentre(), axes[2]);

			glm::vec3 vecToEdge = glm::vec3(0, 0, 0);

			// Now check each axis
			for(int i = 0; i < 3; ++i)
			{
				/*
				if (center.x < bmin.x) {
					dmin += Math.pow(center.x - bmin.x, 2);
				} else if (center.x > bmax.x) {
					dmin += Math.pow(center.x - bmax.x, 2);
				}

				if (center.y < bmin.y) {
					dmin += Math.pow(center.y - bmin.y, 2);
				} else if (center.y > bmax.y) {
					dmin += Math.pow(center.y - bmax.y, 2);
				}

				if (center.z < bmin.z) {
					dmin += Math.pow(center.z - bmin.z, 2);
				} else if (center.z > bmax.z) {
					dmin += Math.pow(center.z - bmax.z, 2);
				}
				*/

				// "Left" and "Right" just to help me visualize, actually a 1D axis could be any direction  (depends on orientation of box)
				if(sphCentreDist[i] < boxMinMax[i].x) // less than box "left" side x==min
				{
					vecToEdge[i] = boxMinMax[i].x - sphCentreDist[i]; // point from centre to "left" edge
				}
				else if(sphCentreDist[i] > boxMinMax[i].y) // more than box "right" side y==max
				{
					vecToEdge[i] = boxMinMax[i].y - sphCentreDist[i]; // point from centre to "right" edge
				}
				else // is definately colliding
				{
					float leftOverlap =  sphCentreDist[i] - boxMinMax[i].x; // x==min
					float rightOverlap =  boxMinMax[i].x - sphCentreDist[i]; // y==max

					if(leftOverlap > rightOverlap) // then I am on "left" so collision normal positive
					{
						float dist = leftOverlap + _radius;
						if(i == 0 || std::abs(dist) < collInfo.penDepth)
						{
							collInfo.normal = axes[i];
							collInfo.penDepth = dist;
						}
					}
					else // then I am on "right" so collision normal negative
					{
						float dist = rightOverlap + _radius;
						if(i == 0 || std::abs(dist) < collInfo.penDepth)
						{
							collInfo.normal = -axes[i];
							collInfo.penDepth = dist;
						}
					}
				}

			}

			// Ok, checked each axis. If vec to edge is zero then face collision, already worked out collision info. Else we can get it from vec to edge
			if(vecToEdge == glm::vec3(0, 0, 0)) return true;
			

			// Is it acually collding though?
			float squareDistToEdge = glm::dot(vecToEdge, vecToEdge);
			float squareRad = _radius * _radius;
			if(squareDistToEdge < squareRad)
			{
				float distMag = std::sqrt(squareDistToEdge);
				collInfo.bodyA = _physicsBody;
				collInfo.bodyB = other->getPhysicsBody();
				collInfo.penDepth = _radius - distMag;
				collInfo.normal = vecToEdge / distMag;
				return true;
			}
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

void BoxCollider::calculateBounds()
{
	glm::mat4 transMat = getTransformMatrix();

	// near face
	_worldVerts[0] = glm::vec3(transMat * glm::vec4(-0.5f, 0.5f, 0.5f, 1.0)); // tl
	_worldVerts[1] = glm::vec3(transMat * glm::vec4(0.5f, 0.5f, 0.5f, 1.0)); // tr
	_worldVerts[2] = glm::vec3(transMat * glm::vec4(-0.5f, -0.5f, 0.5f, 1.0)); // bl
	_worldVerts[3] = glm::vec3(transMat * glm::vec4(0.5f, -0.5f, 0.5f, 1.0)); // br
	
	// far face
	_worldVerts[4] = glm::vec3(transMat * glm::vec4(0.5f, 0.5f, -0.5f, 1.0)); // tl
	_worldVerts[5] = glm::vec3(transMat * glm::vec4(-0.5f, 0.5f, -0.5f, 1.0)); // tr
	_worldVerts[6] = glm::vec3(transMat * glm::vec4(0.5f, -0.5f, -0.5f, 1.0)); // bl
	_worldVerts[7] = glm::vec3(transMat * glm::vec4(-0.5f, -0.5f, -0.5f, 1.0)); // br

	// Calc bounds
	_bounds.min = _worldVerts[0];
	_bounds.max = _worldVerts[0];
	for(int i = 1; i < 8; ++i)
	{
		_bounds.min.x = std::min(_bounds.min.x, _worldVerts[i].x);
		_bounds.min.y = std::min(_bounds.min.y, _worldVerts[i].y);
		_bounds.min.z = std::min(_bounds.min.z, _worldVerts[i].z);

		_bounds.max.x = std::max(_bounds.min.x, _worldVerts[i].x);
		_bounds.max.y = std::max(_bounds.min.y, _worldVerts[i].y);
		_bounds.max.z = std::max(_bounds.min.z, _worldVerts[i].z);
	}
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
			const glm::vec3* otherVerts = otherB->getWorldVerts();

			// Work out all 15 axes we need to check - if it can be done neatly, would be better to calc as we go to avoid calcs not needed
			// Also might be faster to transform unit fwd, right, up by matrix (omitting scale) assuming normalize uses sqrt
			glm::vec3 axes[15];
			axes[0] = _worldVerts[1] - _worldVerts[0]; // axis myRight
			axes[1] = _worldVerts[0] - _worldVerts[2]; // axis myUp
			axes[2] = _worldVerts[0] - _worldVerts[4]; // axis myBack (Z+, towards us)
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
				float myMin = glm::dot(_worldVerts[0], axes[a]);
				float myMax = myMin;
				float otherMin = glm::dot(otherVerts[0], axes[a]);
				float otherMax = otherMin;

				// For every other vertex
				for(int v = 1; v < 8; ++v)
				{
					// Note that the dot product gives you distance from origin of vert projected on axis
					float myProjDist = glm::dot(_worldVerts[v], axes[a]);
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

				collInfo.bodyA = _physicsBody;
				collInfo.bodyB = other->getPhysicsBody();

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



const glm::vec3* BoxCollider::getWorldVerts()
{
	return &_worldVerts[0];
}

void BoxCollider::test()
{
	// Debug
	const glm::vec3* verts = getWorldVerts();

	std::cout << "WORLD VERTS:\n";
	for(int i = 0; i < 8; ++i)
	{
		std::cout << verts[i].x << ", ";
		std::cout << verts[i].y << ", ";
		std::cout << verts[i].z << "\n";
	}
	std::cout << "\n\n";

}