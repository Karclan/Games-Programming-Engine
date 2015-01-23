#include "physics\physicsSystem.h"

void PhysicsSystem::clear()
{
	_physBodys.clear();
}

void PhysicsSystem::addPhysBody(SPtr_PhysBody physBody)
{
	_physBodys.push_back(physBody);
}

void PhysicsSystem::fixedUpdate(float t)
{
	for(int i = 0; i < _physBodys.size(); i++)
	{
		_physBodys[i]->test();
	}
}