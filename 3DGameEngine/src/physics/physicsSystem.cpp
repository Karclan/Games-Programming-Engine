#include "physics\physicsSystem.h"
#include <math.h>

PhysicsSystem::PhysicsSystem()
{
	tested = false;
	_terrainCollider = nullptr;
}

void PhysicsSystem::init()
{
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
}

void PhysicsSystem::clear()
{
	_terrainCollider = nullptr;
	_unsortedColliders.clear();
	_dynamicColliders.clear();
	_staticColliders.clear();
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
	// Do the integration
	for(int i = 0; i < _dynamicColliders.size(); ++i)
	{
		_dynamicColliders[i]->getPhysicsBody()->fixedUpdate(t);
		_dynamicColliders[i]->calculateBounds();
	}



	// For each collider


	// Put in tree, separate ones with physics bodies

	// Need to consider tree limit - how many physics bodies colliders, how many without?
	// E.g. Must have between 1 and 3 physics bodies in leaf (0 bodies can be ignored)
	// What is max number of regular colliders? Division is surely effectively AABB collision anyway, right?

	

	// FOR TESTING - Adds colliders to single test node - this would be a leaf node in finished version
	_testNode.clear();

	for(int i = 0; i < _dynamicColliders.size(); ++i)
	{
		_testNode.physColliders.push_back(_dynamicColliders[i]);
	}

	for(int i = 0; i < _staticColliders.size(); ++i)
	{
		_testNode.regColliders.push_back(_staticColliders[i]);
	}

	// Now, in each leaf node check collisions
	// If multiple "resolve" type collisions on same object, prioritize
	// Resolve and then remove physic colliders that moved and re-add them to tree
	Collision colInfo;

	for(int i = 0; i < _testNode.physColliders.size(); ++i)
	{
		for(int pc = i+1; pc < _testNode.physColliders.size(); ++pc) // start at i+1 so don't duplicate collision tests
		{
			if(_testNode.physColliders[i]->collides(_testNode.physColliders[pc], colInfo))
			{
				//std::cout << "I collider! " << colInfo.penDepth << "\n";
				colInfo.resolve();

				std::cout << "Pen Depth " << colInfo.penDepth << "\n";
				std::cout << "Normal " << colInfo.normal.x << ", " << colInfo.normal.y << ", " << colInfo.normal.z << "\n\n";


			}
		}

		for(int rc = 0; rc < _testNode.regColliders.size(); ++rc)
		{
			if(_testNode.physColliders[i]->collides(_testNode.regColliders[rc], colInfo))
			{
				colInfo.resolve();
			}
		}

		// And the floor...
		/*
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
		*/
		
	}



	// Repeat x times or until no more collisions
	
}


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