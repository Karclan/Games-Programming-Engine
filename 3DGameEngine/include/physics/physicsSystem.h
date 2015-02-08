#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <vector>

#include "physics\physicsBody.h"

/*! \brief Physics Sub-System

	System for managing physics objects

*/

class PhysicsSystem
{
public:
	PhysicsSystem(){};
	~PhysicsSystem(){};

	void fixedUpdate(float t); //!< Physics uses a fixed timestep
	void clear();
	void addPhysBody(SPtr_PhysBody physBody);

private:
	std::vector<SPtr_PhysBody> _physBodys;

	bool collides(SPtr_Collider colA, SPtr_Collider colB);


};

#endif