#ifndef COLLISION_H
#define COLLISION_H


#include "physics\physicsBody.h"
#include "physics\collider.h"


/*! \brief Collison including info

*/

class Collision
{
public:
	Collision()
	{
		normal = glm::vec3();
		penDepth = 0;
		bodyA = nullptr;
		bodyB = nullptr;
	}

	glm::vec3 normal;
	float penDepth;
	SPtr_PhysBody bodyA;
	SPtr_PhysBody bodyB;

	void resolve();

};


#endif