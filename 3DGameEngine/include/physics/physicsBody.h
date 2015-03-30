#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include <vector>
#include <map>

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "core\component.h"
#include "core\transform.h"


/*! \brief Physics Body for handling collisions


*/
#include <iostream>

class PhysicsBody : public Component
{
public:
	PhysicsBody();
	~PhysicsBody();

	ComponentType::Type getType(); //!< Required implementation. Return type of component
	bool isOnePerObject(); //!< Required implementation. Return true if you can only have one of these per object
	void linkDependency(SPtr_Component component); //!< Override to link needed dependencies, e.g. switch desired types and cache in a variable. Make sure the components have been requested in the dependencyFlags variable.
	void init();

	void fixedUpdate(float t); //!< Fixed update to handle integration etc
	
	void addForce(glm::vec3 force);  //!< Add force (accel)
	void addImpulse(glm::vec3 impulse); //!< Add impulse (instant velocity);

	SPtr_Transform getTransform() { return _transform; }
	bool isAwake() { return _awake; }
	void wakeUp();

	glm::vec3 getVelocity() { return _velocity; }
	void setVelocity(glm::vec3 v);
	
	float getMass() { return _mass; } // when make set mass, ensure to check for <= 0 so not negative or zero
	void setMass(float mass) { _mass = std::max(mass, 0.01f); }

	void setGravity(float gravity) { _gravity = gravity; }
	void setDrag(float drag) { _drag = drag; }

	// Collision functions
	void addCollisionHit(unsigned int objId, glm::vec3 normal);
	void addTriggerHit(unsigned int objId);
	bool hasCollided();
	bool hasTriggered();
	bool isGrounded() { return _isGrounded; }
	const std::map<unsigned int, glm::vec3>* getCollisions() { return &_collisions; }
	const std::vector<int>* getTriggers() { return &_triggers; }

private:
	SPtr_Transform _transform; //!< Pointer to transform
	bool _awake;
	float _sleepTimer;

	float _mass;
	float _drag;
	float _gravity;
	glm::vec3 _accel;
	glm::vec3 _velocity;

	// Collisions!
	std::map<unsigned int, glm::vec3> _collisions;
	std::vector<int> _triggers;
	bool _isGrounded;
	unsigned int _ungroundLatency; //!< Stops instant ungrounding to make up for a few frames of no collisions that would falsely register leaving ground

};

//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<PhysicsBody> SPtr_PhysBody;


// Include after definition of physicsbody class because circular reference
//#include "physics\collision.h"


#endif