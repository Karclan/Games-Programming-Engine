#ifndef COLLISION_H
#define COLLISION_H


#include "physics\physicsBody.h"
#include "physics\collider.h"


/*! \brief Collison including info

*/

class Collision
{
public:
	Collision();

	glm::vec3 normal;
	float penDepth;
	SPtr_PhysBody bodyA;
	SPtr_PhysBody bodyB;

	void resolve();

};


#endif