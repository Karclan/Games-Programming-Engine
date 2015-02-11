#ifndef COLLIDER_H
#define COLLIDER_H

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "core\component.h"
#include "core\transform.h"
#include "physics\physicsBody.h"





// FWD Declarations
class Collider;
class SphereCollider;
class BoxCollider;

typedef std::shared_ptr<Collider> SPtr_Collider;
typedef std::shared_ptr<SphereCollider> SPtr_SphereCol;
typedef std::shared_ptr<BoxCollider> SPtr_BoxCol;

/*! \brief Collider


*/
class Collider : public Component
{
public:
	Collider();

	ComponentType::Type getType()=0; //!< Required implementation. Return type of component
	bool isOnePerObject() { return true; } //!< Currently can only have 1 of each type of collider
	void linkDependency(SPtr_Component component); //!< Override to link needed dependencies, e.g. switch desired types and cache in a variable. Make sure the components have been requested in the dependencyFlags variable.

	bool hasPhysicsBody() { return _physicsBody; } //!< Returns true if physics body exists

	virtual bool collides(SPtr_Collider other)=0;

protected:
	SPtr_Transform _transform; //!< Pointer to transform
	SPtr_PhysBody _physicsBody; //!< Optional pointer to physics body
};





// SPhere Collider
class SphereCollider : public Collider
{
public:
	SphereCollider();

	ComponentType::Type getType(); //!< Required implementation. Return type of component

	bool collides(SPtr_Collider other); //!< Collision logic

	float getRadius();
	glm::vec3 getOffset() { return _offset; }
	glm::vec3 getCentre();

	void setRadius(float radius) { _radius = radius; }
	void setOffset(glm::vec3 offset) { _offset = offset; }

	

private:
	float _radius;
	glm::vec3 _offset;

};





// Box Collider
class BoxCollider : public Collider
{
public:
	BoxCollider();

	ComponentType::Type getType(); //!< Required implementation. Return type of component

	bool collides(SPtr_Collider other); //!< Collision logic


	glm::mat4 getRotationMatrix() { return _transform->getRotationMatrix(); }
	glm::mat4 getTransformMatrix();

	glm::vec3 getExtents();
	glm::vec3 getOffset() { return _offset; }
	glm::vec3 getCentre();
	void setExtents(glm::vec3 extents) { _extents = extents; }
	void setOffset(glm::vec3 offset) { _offset = offset; }

	void test();
	void getWorldVerts(glm::vec3 verts[]);

private:
	glm::vec3 _extents; //!< Width, height and depth of box. Note - not half extents!
	glm::vec3 _offset;

	
};





#endif