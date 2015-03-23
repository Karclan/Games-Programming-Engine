#include "physics\collider.h"


bool AABB::intersects(AABB &other)
{
	if(max.x < other.min.x) return false;
	if(min.x > other.max.x) return false;
	if(max.y < other.min.y) return false;
	if(min.y > other.max.y) return false;
	if(max.z < other.min.z) return false;
	if(min.z > other.max.z) return false;
	return true;
}


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
bool SphereCollider::collides(Collider* other, Collision &collInfo)
{
	if(!_bounds.intersects(other->_bounds)) return false;

	switch(other->getType())
	{
		
	case ComponentType::SPHERE_COL:
		{
			SphereCollider* otherS = static_cast<SphereCollider*>(other);
			glm::vec3 aToB = otherS->getCentre() - getCentre();
			float squareDist = glm::dot(aToB, aToB); // i think this would give you x*x + y*y + z*z !
			float radSum = _radius + otherS->getRadius();
			float squareRadSum = radSum * radSum;


			if(squareDist < squareRadSum)
			{
				float distMag = std::sqrt(squareDist);
				float penDepth = radSum - distMag;

				if(penDepth > collInfo.penDepth)
				{
					collInfo.bodyA = _physicsBody;
					collInfo.bodyB = other->getPhysicsBody();
					collInfo.penDepth = penDepth;
					collInfo.normal = aToB / distMag;
				}
				return true;
			}
			return false;
		}
		
	case ComponentType::BOX_COL:
		{
			float penDepth = 0;
			glm::vec3 normal;

			BoxCollider* otherB = static_cast<BoxCollider*>(other);

			// Get other's verts in world space
			const glm::vec3* otherVerts = otherB->getWorldVerts();

			// Work out axes
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
				// "Left" and "Right" just to help me visualize, actually a 1D axis could be any direction  (depends on orientation of box)
				if(sphCentreDist[i] < boxMinMax[i].x) // less than box "left" side x==min
				{
					vecToEdge += axes[i] * (sphCentreDist[i] - boxMinMax[i].x); // axis scaled by distance from centre to "left" edge
				}
				else if(sphCentreDist[i] > boxMinMax[i].y) // more than box "right" side y==max
				{
					vecToEdge += axes[i] * (sphCentreDist[i] - boxMinMax[i].y); // point from centre to "right" edge
				}
				else // is definately colliding
				{
					// Not sure this works. This code only ever runs if massive overlap (the centre of the ball
					// would have to be within the box on every axis).
					float leftOverlap =  sphCentreDist[i] - boxMinMax[i].x; // x==min
					float rightOverlap =  boxMinMax[i].x - sphCentreDist[i]; // y==max

					if(leftOverlap > rightOverlap) // then I am on "left" so collision normal positive
					{
						float dist = leftOverlap + _radius;
						if(i == 0 || std::abs(dist) < penDepth)
						{
							normal = axes[i];
							penDepth = dist;
						}
					}
					else // then I am on "right" so collision normal negative
					{
						float dist = rightOverlap + _radius;
						if(i == 0 || std::abs(dist) < penDepth)
						{
							normal = -axes[i];
							penDepth = dist;
						}
					}


				}

			}

			// Ok, checked each axis. If vec to edge is zero then face collision, already worked out collision info. Else we can get it from vec to edge
			
			if(vecToEdge == glm::vec3(0, 0, 0))
			{
				if(penDepth > collInfo.penDepth)
				{
					collInfo.bodyA = _physicsBody;
					collInfo.bodyB = other->getPhysicsBody();
					collInfo.penDepth = penDepth;
					collInfo.normal = normal;
				}
				return true;
			}
			
			
			
			// Use vec to edge to deterine if colliding and collision info
			float squareDistToEdge = glm::dot(vecToEdge, vecToEdge);
			float squareRad = _radius * _radius;
			if(squareDistToEdge < squareRad)
			{
				float distMag = std::sqrt(squareDistToEdge);
				penDepth = _radius - distMag;

				if(penDepth > collInfo.penDepth)
				{
					collInfo.bodyA = _physicsBody;
					collInfo.bodyB = other->getPhysicsBody();
					collInfo.penDepth = penDepth;
					collInfo.normal = -vecToEdge / distMag;
				}
				return true;
			}
			
			return false;
		}
		break;

		// Just to shut compiler warning up
		default:
			return false;
	}
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
	_worldVerts[0] = glm::vec3(transMat * glm::vec4(-0.5f, 0.5f, 0.5f, 1.0)); // tlf
	_worldVerts[1] = glm::vec3(transMat * glm::vec4(0.5f, 0.5f, 0.5f, 1.0)); // trf
	_worldVerts[2] = glm::vec3(transMat * glm::vec4(-0.5f, -0.5f, 0.5f, 1.0)); // blf
	_worldVerts[3] = glm::vec3(transMat * glm::vec4(0.5f, -0.5f, 0.5f, 1.0)); // brf
	
	// far face
	_worldVerts[4] = glm::vec3(transMat * glm::vec4(-0.5f, 0.5f, -0.5f, 1.0)); // tlb
	_worldVerts[5] = glm::vec3(transMat * glm::vec4(0.5f, 0.5f, -0.5f, 1.0)); // trb
	_worldVerts[6] = glm::vec3(transMat * glm::vec4(-0.5f, -0.5f, -0.5f, 1.0)); // blb
	_worldVerts[7] = glm::vec3(transMat * glm::vec4(0.5f, -0.5f, -0.5f, 1.0)); // brb

	// Calc bounds
	_bounds.min = _worldVerts[0];
	_bounds.max = _worldVerts[0];
	for(int i = 1; i < 8; ++i)
	{
		_bounds.min.x = std::min(_bounds.min.x, _worldVerts[i].x);
		_bounds.min.y = std::min(_bounds.min.y, _worldVerts[i].y);
		_bounds.min.z = std::min(_bounds.min.z, _worldVerts[i].z);

		_bounds.max.x = std::max(_bounds.max.x, _worldVerts[i].x);
		_bounds.max.y = std::max(_bounds.max.y, _worldVerts[i].y);
		_bounds.max.z = std::max(_bounds.max.z, _worldVerts[i].z);
	}
}


// Wonderful collision logic!
bool BoxCollider::collides(Collider* other, Collision &collInfo)
{
	if(!_bounds.intersects(other->_bounds)) return false;

	switch(other->getType())
	{
	case ComponentType::SPHERE_COL:
		{
			float penDepth = collInfo.penDepth;
			if(other->collides(this, collInfo))
			{
				if(penDepth < collInfo.penDepth)
				{
					// The switch-a-roo!
					SPtr_PhysBody temp = collInfo.bodyA;
					collInfo.bodyA = collInfo.bodyB;
					collInfo.bodyB = temp;
					collInfo.normal = -collInfo.normal;
				}

				return true;
			}
			return false;
		}

	case ComponentType::BOX_COL:
		{
			BoxCollider* otherB = static_cast<BoxCollider*>(other);
			
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


			// To store normal and pen depth
			float penDepth = 0;
			glm::vec3 normal;
			
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

				if(overlap < invOverlap) // then I am on "left" so collision normal positive
				{
					if(a == 0 || std::abs(overlap) < penDepth)
					{
						normal = axes[a];
						penDepth = overlap;
					}
				}
				else // then I am on "right" so collision normal negative
				{
					if(a == 0 || std::abs(invOverlap) < penDepth)
					{
						normal = -axes[a];
						penDepth = invOverlap;
					}
				}
			}

			// couldn't find axis of separation, they collide!
			// Only set col info if pen depth is bigger
			if(penDepth > collInfo.penDepth)
			{
				collInfo.bodyA = _physicsBody;
				collInfo.bodyB = other->getPhysicsBody();
				collInfo.normal = normal;
				collInfo.penDepth = penDepth;
			}
			return true; 
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