#ifndef RENDERER_H
#define RENDERER_H

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "core\component.h"
#include "core\transform.h"

/*! \brief Base class for render objects

	Base class for anything that is rendered in 3D space
*/
class Renderer : public Component
{
public:
	Renderer();
	virtual ComponentType::Type getType() = 0;
	bool isOnePerObject() = 0;

	virtual void render(GLfloat* viewMatrix, GLfloat* projMatrix) = 0; //!< Note the args are pointers to the start of the float arrays where the matrix information is stored
	virtual void animate(float t){} //!< Empty rather than pure virtual as not every model requires implementation. Unanimatables just need blank functions for polymorphism.

protected:
	SPtr_Transform _transform; //!< Pointer to transform component. Required for rendering.


};

//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<Renderer> SPtr_Renderer;



#endif