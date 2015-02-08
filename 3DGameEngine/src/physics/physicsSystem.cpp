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
	// Rubbish collision detection with no broad phase just to get things started
	for(int i = 0; i < _physBodys.size(); ++i)
	{
		const std::vector<SPtr_Collider>* colliders = _physBodys[i]->getColliders();
		if(colliders->size() == 0) continue; // no colliders here!

		for(int j = 0; j < _physBodys.size(); ++j)
		{
			// if any of the colliders are collding!
			for(int k = 0; k < _physBodys[j]->getColliders()->size(); ++k)
			{
				
			}

		}
		_physBodys[i]->test();
	}
}


bool PhysicsSystem::collides(SPtr_Collider colA, SPtr_Collider colB)
{
	return false;
}