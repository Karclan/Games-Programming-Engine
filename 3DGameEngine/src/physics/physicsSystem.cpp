#include "physics\physicsSystem.h"
#include <math.h>

PhysicsSystem::PhysicsSystem()
{
	_maxIterations = 5;
	_terrainCollider = nullptr;


	//_staticOctTree.setMaxDepth(0); // i think the oct tree makes static stuff worse! Uniform grid would be better (probs because only a handful of static colliders atm so oct tree has adverse affect)

}

void PhysicsSystem::init()
{
	// Clear Dynamic OctTree (ststic will be done automatically when it is created
	_dynamicOctTree.clear();

	// Ensure all colliders are in unsorted colliders
	for(int i = 0; i < _dynamicColliders.size(); ++i)
		_unsortedColliders.push_back(_dynamicColliders[i]);

	for(int i = 0; i < _staticColliders.size(); ++i) 
		_unsortedColliders.push_back(_staticColliders[i]);

	// Clear dynamic and static colliders
	_dynamicColliders.clear();
	_staticColliders.clear();

	// Sort and init colliders
	for(int i = 0; i < _unsortedColliders.size(); ++i)
	{
		// init
		_unsortedColliders[i]->calculateBounds();


		// Sort
		if(_unsortedColliders[i]->hasPhysicsBody())
			_dynamicColliders.push_back(_unsortedColliders[i]);
		else
			_staticColliders.push_back(_unsortedColliders[i]);
	}

	// Clear init list
	_unsortedColliders.clear();

	// Now construct oct tree with static colliders
	_staticOctTree.createTree(_staticColliders);
}

void PhysicsSystem::clear()
{
	_terrainCollider = nullptr;
	_unsortedColliders.clear();
	_dynamicColliders.clear();
	_staticColliders.clear();

	_dynamicOctTree.clear();
	_staticOctTree.clear();
}

void PhysicsSystem::addCollider(SPtr_Collider collider)
{
	_unsortedColliders.push_back(collider);
}

void PhysicsSystem::addTerrainCollider(SPtr_TerrainCol collider)
{
	_terrainCollider = collider;
}

void PhysicsSystem::fixedUpdate(float t)
{
	// If dynamically created object
	if(_unsortedColliders.size() != 0)
	{
		for(int i = 0; i < _unsortedColliders.size(); ++i)
		{
			// Sort
			if(_unsortedColliders[i]->hasPhysicsBody())
				_dynamicColliders.push_back(_unsortedColliders[i]);
			else
			{
				_staticColliders.push_back(_unsortedColliders[i]);
				_staticOctTree.createTree(_staticColliders);
			}
		}

		// Clear init list
		_unsortedColliders.clear();
	}


	// Do the integration
	for(int i = 0; i < _dynamicColliders.size(); ++i)
	{
		if(!_dynamicColliders[i]->getPhysicsBody()->isAwake()) continue;
		_dynamicColliders[i]->getPhysicsBody()->fixedUpdate(t);
		_dynamicColliders[i]->calculateBounds();
	}

	// Do the oct tree
	_dynamicOctTree.createTree(_dynamicColliders);
	std::map<SPtr_Collider, std::set<SPtr_Collider>> collisionMatrix;
	_dynamicOctTree.getCollisionMatrix(collisionMatrix);

	// Use collsion matrix to determine possible collisions
	std::map<SPtr_Collider, std::set<SPtr_Collider>>::iterator mapIt;
	std::set<SPtr_Collider>::iterator setIt;
	bool checkAgain = false; // should collisions be checked again after iteration? (provided not reached max iters)
	bool invResolution = false; // if true, pushes B out of collision rather than A.
	for(int i = 0; i < _maxIterations; ++i)
	{
		// STATIC COLLISIONS! ##
		for(int dc = 0; dc < _dynamicColliders.size(); ++dc)
		{
			if(!_dynamicColliders[dc]->getPhysicsBody()->isAwake()) continue;
			Collision colInfoS; // ensure col info is unique or initialized to zero (pen depth) for each dynamic object checked
			std::set<SPtr_Collider> staticCols;
			_staticOctTree.getCollidersFromAABB(_dynamicColliders[dc]->getBounds(), staticCols);

			// For every static collider
			for(setIt = staticCols.begin(); setIt != staticCols.end(); ++setIt)
			{
				checkCollision(_dynamicColliders[dc], *setIt, colInfoS);
			}

			if(colInfoS.penDepth > 0)
			{
				resolveCollision(colInfoS);
				checkAgain = true;
			}
		}


		// DYNAMIC COLLISIONS!
		for(mapIt = collisionMatrix.begin(); mapIt != collisionMatrix.end(); ++mapIt) // for each collidable...
		{
			Collision colInfoD = Collision();

			// Check dynamic collisions
			for(setIt = mapIt->second.begin(); setIt != mapIt->second.end(); ++setIt)
			{
				// Because will only set col info if pen depth larger than current, we will end up with most significant collision
				if(!invResolution)
					checkCollision(mapIt->first, *setIt, colInfoD);
				else
					checkCollision(*setIt, mapIt->first, colInfoD);

			}

			// Resolve collision
			if(colInfoD.penDepth > 0)
			{
				resolveCollision(colInfoD);
				checkAgain = true;
			}
		}

		
		

		if(!checkAgain) break;
		invResolution = !invResolution;
	}

	

	// Temp Static collision stuff
	/*
	for(int i = 0; i < _dynamicColliders.size(); ++i)
	{
		Collision colInfo; // ensure col info is unique or initialized to zero (pen depth) for each dynamic object checked
		for(int rc = 0; rc < _staticColliders.size(); ++rc)
		{
			if(_dynamicColliders[i]->collides(_staticColliders[rc].get(), colInfo))
			{
				colInfo.resolve();
			}
		}

		// And the floor...
		
		if(_terrainCollider != nullptr)
		{
			if(_terrainCollider->collides(_testNode.physColliders[i], colInfo))
			{
				//std::cout << "I collider! " << colInfo.penDepth << "\n";
				_testNode.physColliders[i]->getPhysicsBody()->resolve(colInfo);

				std::cout << "Pen Depth " << colInfo.penDepth << "\n";
				std::cout << "Normal " << colInfo.normal.x << ", " << colInfo.normal.y << ", " << colInfo.normal.z << "\n\n";


			}

		}
		
		
	}
	*/


	// Repeat x times or until no more collisions
	
}

void PhysicsSystem::checkCollision(SPtr_Collider a, SPtr_Collider b, Collision &colInfo)
{
	if(a->collides(b.get(), colInfo))
	{
		colInfo.colliderA = a;
		colInfo.colliderB = b;
	}
}


void PhysicsSystem::resolveCollision(Collision &collision)
{
	SPtr_PhysBody bodyA = collision.colliderA->getPhysicsBody();
	SPtr_PhysBody bodyB = collision.colliderB->getPhysicsBody();

	// Body A should be the body being checked, so if it's null then something's gone wrong!
	if(bodyA == nullptr) return;




	// RESOLVE -- BUTTERS SIMPLE FOR NOW
	SPtr_Transform transA = bodyA->getTransform();
	transA->translate(collision.normal * -collision.penDepth);





	// RESPOND -- IMPULSE
	// If body B is null, this is a collision with a static entity
	float friction = std::min(std::max(collision.colliderA->getFriction(), collision.colliderB->getFriction()), 1.0f);
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
	float j = -(1+e) * glm::dot(relVel, collision.normal) / sumInvMass;
	glm::vec3 jn = collision.normal * j;

	bodyA->setVelocity((bodyA->getVelocity() - jn/bodyA->getMass()) * friction);
	bodyA->addCollisionHit(collision.colliderB->getObjectID(), -collision.normal);
	collision.colliderA->calculateBounds();
	if(bodyB != nullptr)
	{
		bodyB->setVelocity((bodyB->getVelocity() + jn/bodyB->getMass()) * friction);
		bodyB->addCollisionHit(collision.colliderA->getObjectID(), collision.normal);
		collision.colliderB->calculateBounds();
	}
}





// Remove objects
void PhysicsSystem::removeCollider(SPtr_Collider collider)
{
	// If in unsorted list
	if(_unsortedColliders.size() != 0)
	{
		std::vector<SPtr_Collider>::iterator it = _unsortedColliders.begin();
		for(it; it != _unsortedColliders.end(); ++it)
		{
			if(*it == collider)
			{
				_unsortedColliders.erase(it);
				return;
			}
		}
	}

	// Else have to determine if dynamic or not, if static must also delete from static tree
	if(collider->hasPhysicsBody())
	{
		std::vector<SPtr_Collider>::iterator it = _dynamicColliders.begin();
		/*
		while(it != _dynamicColliders.end())
		{
			if(*it == collider)
			{
				it = _dynamicColliders.erase(it);
			}
			else
			{
				++it;
			}
		}
		*/

		int counter = 0;
		for(it; it != _dynamicColliders.end(); ++it)
		{
			if(*it == collider)	
			{
				counter++;
				_dynamicColliders.erase(it);
				break;
			}
			
		}
		std::cout << "Found that shit " << counter << " times lol!\n";

		counter = 0;
		for(it = _dynamicColliders.begin(); it != _dynamicColliders.end(); ++it)
		{
			if(*it == collider) counter++;
		}
		std::cout << "Looked again. Found that shit " << counter << " times lol!\n";

	}

	// Must be static

	
}

void PhysicsSystem::removeTerrainCollider(SPtr_TerrainCol collider)
{
	_terrainCollider = SPtr_TerrainCol(nullptr);
}









// Rendering of colliders (for debug, so can position them in real time)
void PhysicsSystem::renderColliders(Camera* camera)
{
	for(int i = 0; i < _unsortedColliders.size(); ++i)
		renderCollider(_unsortedColliders[i], camera);

	for(int i = 0; i < _dynamicColliders.size(); ++i)
		renderCollider(_dynamicColliders[i], camera);
	
	for(int i = 0; i < _staticColliders.size(); ++i)
		renderCollider(_staticColliders[i], camera);
}





// Render me a loively collider
void PhysicsSystem::renderCollider(SPtr_Collider collider, Camera* camera)
{
	switch(collider->getType())
	{
	case ComponentType::SPHERE_COL:
		{
			SPtr_SphereCol sphereCol = std::static_pointer_cast<SphereCollider>(collider);
			renderSphere(camera, sphereCol->getRadius(), sphereCol->getCentre());
		}
		break;

	case ComponentType::BOX_COL:
		{
			SPtr_BoxCol boxCol = std::static_pointer_cast<BoxCollider>(collider);
			renderBox(camera, boxCol->getTransformMatrix());
		}
		break;
	}
}


// Functions that probably should be put in another class! Maybe debug class?
void PhysicsSystem::renderSphere(Camera* camera, float radius, glm::vec3 pos)
{
	Mesh* mesh = Assets::getMesh("sphere");
	Material mat;
	mat.setShader(Assets::getShader("collider"));


	glm::mat4 modelMat = glm::translate(pos);
	float scale = radius * 2;
	modelMat *= glm::scale(glm::vec3(scale, scale, scale));


	mat.bind(modelMat, camera->getView(), camera->getProjection());
	glBindVertexArray(mesh->getVao());
	glDrawElements(GL_TRIANGLES, mesh->numIndices(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(NULL);
}



void PhysicsSystem::renderBox(Camera* camera, glm::vec3 extents, glm::vec3 pos)
{
	renderBox(camera, extents, pos, glm::mat4());
}
void PhysicsSystem::renderBox(Camera* camera, glm::vec3 extents, glm::vec3 pos, glm::mat4 rot)
{
	Mesh* mesh = Assets::getMesh("cube");
	Material mat;
	mat.setShader(Assets::getShader("collider"));


	glm::mat4 modelMat = glm::translate(pos);
	modelMat *= rot;
	modelMat *= glm::scale(extents);


	mat.bind(modelMat, camera->getView(), camera->getProjection());
	glBindVertexArray(mesh->getVao());
	glDrawElements(GL_TRIANGLES, mesh->numIndices(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(NULL);
}


void PhysicsSystem::renderBox(Camera* camera, const glm::mat4 &transform)
{
	Mesh* mesh = Assets::getMesh("cube");
	Material mat;
	mat.setShader(Assets::getShader("collider"));

	mat.bind(transform, camera->getView(), camera->getProjection());
	glBindVertexArray(mesh->getVao());
	glDrawElements(GL_TRIANGLES, mesh->numIndices(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(NULL);

}




