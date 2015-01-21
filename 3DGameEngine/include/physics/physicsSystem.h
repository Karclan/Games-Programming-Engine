#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <vector>

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

private:


};

#endif