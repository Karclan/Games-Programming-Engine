#ifndef COLLIDER_H
#define COLLIDER_H

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "core\component.h"
#include "core\transform.h"
#include "physics\physicsBody.h"
//#include "physics\collision.h"




// FWD Declarations
class Collider;
class SphereCollider;
class BoxCollider;
class Collision;

typedef std::shared_ptr<Collider> SPtr_Collider;
typedef std::shared_ptr<SphereCollider> SPtr_SphereCol;
typedef std::shared_ptr<BoxCollider> SPtr_BoxCol;


//*! \brief AABB struct for initial checks before more expensive checks
struct AABB
{
	glm::vec3 min;
	glm::vec3 max;

	// for debug yo
	void print()
	{
		std::cout << "X: " << min.x << " to " << max.x << "\n";
		std::cout << "Y: " << min.y << " to " << max.y << "\n";
		std::cout << "Z: " << min.z << " to " << max.z << "\n";
	}

	bool intersects(AABB &other);
};





/*! \brief Collision Class for storing info about collision events
*/
class Collision
{
public:
	Collision();

	glm::vec3 normal;
	float penDepth;
	SPtr_Collider colliderA;
	SPtr_Collider colliderB;
};





/*! \brief Collider
*/
class Collider : public Component
{
	// Friends means can access things like bounds faster
	friend class SphereCollider;
	friend class BoxCollider;

public:
	Collider();

	ComponentType::Type getType()=0; //!< Required implementation. Return type of component
	bool isOnePerObject() { return false; } //!< Can have numerous colliders, BUT only first found will work with physics body. So use 1 for dynamic objects, many for static objects
	void linkDependency(SPtr_Component component); //!< Override to link needed dependencies, e.g. switch desired types and cache in a variable. Make sure the components have been requested in the dependencyFlags variable.
	
	

	bool hasPhysicsBody() { return _physicsBody; } //!< Returns true if physics body exists
	SPtr_PhysBody getPhysicsBody() { return _physicsBody; }

	virtual bool collides(Collider* other, Collision &collInfo)=0;

	glm::vec3 getRight() { return _transform->getForward(); }
	glm::vec3 getUp() { return _transform->getUp(); }
	glm::vec3 getForward() { return _transform->getForward(); }

	virtual void calculateBounds() = 0; //!< Should be called on init and every frame if has a physics body
	AABB getBounds() { return _bounds; }
	void setFriction(float friction);
	float getFriction() { return _friction; }

protected:
	SPtr_Transform _transform; //!< Pointer to transform
	SPtr_PhysBody _physicsBody; //!< Optional pointer to physics body

	AABB _bounds; //!< Bounding box in world coordinates
	float _friction;
	
};





// SPhere Collider
class SphereCollider : public Collider
{

public:
	SphereCollider();
	~SphereCollider();

	ComponentType::Type getType(); //!< Required implementation. Return type of component

	void calculateBounds();
	bool collides(Collider* other, Collision &collInfo); //!< Collision logic

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
	~BoxCollider();

	ComponentType::Type getType(); //!< Required implementation. Return type of component
	void calculateBounds();
	bool collides(Collider* other, Collision &collInfo); //!< Collision logic


	glm::mat4 getRotationMatrix() { return _transform->getRotationMatrix(); }
	glm::mat4 getTransformMatrix();

	glm::vec3 getExtents();
	glm::vec3 getOffset() { return _offset; }
	glm::vec3 getCentre();
	void setExtents(glm::vec3 extents) { _extents = extents; }
	void setOffset(glm::vec3 offset) { _offset = offset; }

	void test();
	const glm::vec3* getWorldVerts();

private:
	glm::vec3 _extents; //!< Width, height and depth of box. Note - not half extents!
	glm::vec3 _offset;
	glm::vec3 _worldVerts[8]; //!< Position of each corner in world space
	
};

// Include after definition of collider classes because circular reference
//#include "physics\collision.h"


#endif