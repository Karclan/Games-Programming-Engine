#include "physics\collision.h"




/*
void Collision::resolve()
{
	// Body A should be the body being checked, so if it's null then something's gone wrong!
	if(bodyA == nullptr) return;




	// RESOLVE -- BUTTERS SIMPLE FOR NOW
	SPtr_Transform transA = bodyA->getTransform();
	transA->translate(normal * -penDepth);





	// RESPOND -- IMPULSE
	// If body B is null, this is a collision with a static entity
	float friction = 1.0f;
	glm::vec3 veloB = glm::vec3();
	float invMassB = 0;
	if(bodyB != nullptr)
	{
		veloB = bodyB->getVelocity();
		invMassB = 1/bodyB->getMass(); // cos everything has a mass of 1 at the moment
	}

	float sumInvMass = (1/bodyA->getMass()) + invMassB;
	glm::vec3 relVel = veloB - bodyA->getVelocity();
	float e = 0; // coefficient of elasticity. Set to zero for time being to model dullest collision possible (no bounce)

	// Oh boy! Time to work out J!
	float j = -(1+e) * glm::dot(relVel, normal) / sumInvMass;
	glm::vec3 jn = normal * j;

	bodyA->setVelocity((bodyA->getVelocity() - jn/bodyA->getMass()) * friction);
	if(bodyB != nullptr)
	{
		bodyB->setVelocity((bodyB->getVelocity() + jn/bodyB->getMass()) * friction);
	}
}
*/