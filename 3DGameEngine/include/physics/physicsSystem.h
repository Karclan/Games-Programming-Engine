#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <vector>

#include "physics\physicsBody.h"
#include "physics\collider.h"
#include "physics\octTree.h"
#include "core\input.h" // for debug purposes

// For debug shapes
#include "core\assets.h"
#include "rendering\camera.h"
#include "physics\terrainCollider.h"

/*! \brief Physics Sub-System

	System for managing physics objects

*/

class PhysicsSystem
{
public:
	PhysicsSystem();

	void init();
	void fixedUpdate(float t); //!< Physics uses a fixed timestep
	void clear();
	void addCollider(SPtr_Collider collider);
	void addTerrainCollider(SPtr_TerrainCol collider);

	void renderColliders(Camera* camera);
	

private:
	unsigned int _maxIterations; // maximum number of iterations through collision algorithm per frame

	SPtr_TerrainCol _terrainCollider; // terrain collider if any
	
	std::vector<SPtr_Collider> _unsortedColliders;
	std::vector<SPtr_Collider> _dynamicColliders;
	std::vector<SPtr_Collider> _staticColliders;

	OctTree _dynamicOctTree; //!< Oct Tree for testing dynamic colliders


	// Draw debug shapes - would be nicer to outsource this to dedicated debug class or something but this works for now 
	void renderCollider(SPtr_Collider collider, Camera* camera);
	void renderSphere(Camera* camera, float radius, glm::vec3 pos);
	void renderBox(Camera* camera, glm::vec3 extents, glm::vec3 pos);
	void renderBox(Camera* camera, glm::vec3 extents, glm::vec3 pos, glm::mat4 rot);
	void renderBox(Camera* camera, const glm::mat4 &transform);

};

#endif