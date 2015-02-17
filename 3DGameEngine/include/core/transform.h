#ifndef TRANSFORM_H
#define TRANSFORM_H

#define GLM_FORCE_RADIANS

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <memory>
#include <glm/gtx/transform.hpp>

#include "core\component.h"

namespace Direction
{
	const glm::vec3 FORWARD(0, 0, -1);
	const glm::vec3 RIGHT(1, 0, 0);
	const glm::vec3 UP(0, 1, 0);
}


/*! \brief Transform representing position, rotation and scale in 3D space

	A component rather than fundamental property of game object as some
	game objects won't need them (e.g. sound-only, behaviour-only, 2D GUI etc)

	IMPORTANT NOTE: The rotateAround() and lookAt() functions are harder to implement than I
	first thought because matrices need to be decoupled from the 3 vectors for position, euler
	angles and scale. For example, you might want to set position separately from rotation. There needs
	to be a way of converting from the matrix to these 3 vectos and back again.

	The other big problem I encounted was rotation around a point because to get it working intuitvely
	you need to be able to rotate around a relative axis (rather than world axis). This should be solved
	when quaternions are used rather than euler angles but that is beyond the scope of the inital prototype.
	This is an issue for the editor camera, which needs to be able to rotate around it's focus point (and 
	constantly look at it). This is not hard to do with matrices alone but I'm currently unsure of how to update
	the position, rotation and scale vectors to reflect the matrices


*/
class Transform : public Component
{
public:
	Transform();

	// Overridden pure virtuals in component
	ComponentType::Type getType();
	bool isOnePerObject();

	// Data
	glm::mat4 getMatrix(); //!< Returns a copy of the glm::matrix. For openGL use getFloatPointer() instead
	GLfloat* getFloatPointer(); //!< Get matrix as a pointer to first element. Much better to pass around like this for any openGL related stuff
	
	// Transform functions
	
	void setPosition(glm::vec3 pos);
	void setEulerAngles(glm::vec3 ang);
	void setScale(glm::vec3 scale);
	glm::vec3 getPosition();
	glm::vec3 getEulerAngles();
	glm::vec3 getScale();

	void translate(glm::vec3 translation);
	void rotate(glm::vec3 eulerAngles);

	glm::mat4 getRotationMatrix(); //!< Public so box collider can use. May be temp
	
	// Unimplemented functions (see note in description)
	void rotateAround(glm::vec3 eulerAngles, glm::vec3 point);
	void lookAt(glm::vec3 target);
	
	// Direction functions
	glm::vec3 getForward();
	glm::vec3 getRight();
	glm::vec3 getUp();

private:
	// VARIABLES
	glm::mat4 _matrix; //!< Transform as a 4x4 matrix
	
	
	glm::vec3 _position; //!< Position as a vector3
	glm::vec3 _rotation; //!< Rotation euler angles. Consider quaternions in the future!
	glm::vec3 _scale; //!< Scale as a vec3
	


	// FUNCTIONS
	
	void recalculateMatrix(); //!< Call whenever there is a change to pos, rot or scale

};


//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<Transform> SPtr_Transform;

#endif