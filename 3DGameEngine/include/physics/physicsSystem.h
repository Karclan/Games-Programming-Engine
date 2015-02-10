#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <vector>

#include "physics\physicsBody.h"
#include "physics\collider.h"
#include "physics\octTree.h"

// For debug shapes
#include "core\assets.h"
#include "rendering\camera.h"

/*! \brief Physics Sub-System

	System for managing physics objects

*/

class PhysicsSystem
{
public:

	void fixedUpdate(float t); //!< Physics uses a fixed timestep
	void clear();
	void addCollider(SPtr_Collider collider);

	void renderColliders(Camera* camera);

private:
	std::vector<SPtr_Collider> _colliders;

	OctTreeNode _testNode;


	// Draw debug shapes - would be nicer to outsource this to dedicated debug class or something but this works for now 
	void renderSphere(Camera* camera, float radius, glm::vec3 pos);
	void renderBox(Camera* camera, glm::vec3 extents, glm::vec3 pos);
	void renderBox(Camera* camera, glm::vec3 extents, glm::vec3 pos, glm::mat4 rot);
	void renderBox(Camera* camera, glm::vec3 extents, const glm::mat4 &transform);

};

#endif