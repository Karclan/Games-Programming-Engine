#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "core\component.h"
#include "rendering\mesh.h"
#include "rendering\renderer.h"
#include "core\transform.h"

/*! \brief Camera that can render 3D models to the screen

	Rendering a 3D object (model renderer)  will draw it taking this camera's
	view matrix and projection matrix into consideration
*/

class Camera : public Component
{
public:
	//Camera(SPtr_Transform transform); //!< Requires transform
	Camera();

	ComponentType::Type getType(); //!< Required implementation. Return type of component
	bool isOnePerObject(); //!< Required implementation. Return true if you can only have one of these per object
	
	void linkDependency(SPtr_Component component); //!< Override to link needed dependencies, e.g. switch desired types and cache in a variable. Make sure the components have been requested in the dependencyFlags variable.
	
	void preRender(); //!< Sort matrix out
	void render(Renderer &renderer);
	
	
	// Used for drawing shapes without camera
	GLfloat* getView();
	GLfloat* getProjection();


private:
	SPtr_Transform _transform; //!< Pointer to transform
	glm::mat4 _viewMatrix; //!< Transform matrix for the view (pos and rotation of camera)
	glm::mat4 _projectionMatrix; //!< Transform matrix for projection


};

//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<Camera> SPtr_Camera;

#endif