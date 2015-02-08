#ifndef COLLIDER_H
#define COLLIDER_H

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "core\component.h"
#include "core\transform.h"

/*! \brief Collider


*/
#include <iostream>

class Collider : public Component
{
public:
	Collider();

	ComponentType::Type getType()=0; //!< Required implementation. Return type of component
	bool isOnePerObject() { return true; } //!< Currently can only have 1 of each type of collider
	void linkDependency(SPtr_Component component); //!< Override to link needed dependencies, e.g. switch desired types and cache in a variable. Make sure the components have been requested in the dependencyFlags variable.

	virtual void test()=0;

protected:
	SPtr_Transform _transform; //!< Pointer to transform
};

typedef std::shared_ptr<Collider> SPtr_Collider;



// SPhere Collider
class SphereCollider : public Collider
{
public:
	ComponentType::Type getType(); //!< Required implementation. Return type of component

	void test();

};

typedef std::shared_ptr<SphereCollider> SPtr_SphereCol;



#endif