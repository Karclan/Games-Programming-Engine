#include "core\transform.h"
#include <iostream>

Transform::Transform()
{
	_position = glm::vec3(0, 0, 0);
	_rotation = glm::vec3(0, 0, 0);
	_scale = glm::vec3(1, 1, 1);
}


// Overridden pure virtuals in component
ComponentType::Type Transform::getType()
{
	return ComponentType::TRANSFORM;
}

bool Transform::isOnePerObject()
{
	return true;
}
//--------------------------------------


// Data access functions
glm::mat4 Transform::getMatrix()
{
	return _matrix;
}

GLfloat* Transform::getFloatPointer()
{
	return glm::value_ptr(_matrix);
}
//--------------------------------------



// Transform functions
void Transform::setPosition(glm::vec3 pos)
{
	_position = pos;
	recalculateMatrix();
}

void Transform::setEulerAngles(glm::vec3 ang)
{
	_rotation = ang;
	recalculateMatrix();
}

void Transform::setScale(glm::vec3 scale)
{
	_scale = scale;
	recalculateMatrix();
}

glm::vec3 Transform::getPosition()
{
	return _position;
}

glm::vec3 Transform::getEulerAngles()
{
	return _rotation;
}

glm::vec3 Transform::getScale()
{
	return _scale;
}

void Transform::translate(glm::vec3 translation)
{
	_position += translation;
	recalculateMatrix();
}

void Transform::rotate(glm::vec3 eulerAngles)
{
	_rotation += eulerAngles;
	recalculateMatrix();
}

//--------------------------------------


// These functions are not yet implemented - see class description in header file for details
void Transform::rotateAround(glm::vec3 eulerAngles, glm::vec3 point)
{
}

void Transform::lookAt(glm::vec3 target)
{

}




// Direction functions
glm::vec3 Transform::getForward()
{
	glm::vec4 fwd(Direction::FORWARD, 1.0f);
	return glm::vec3(getRotationMatrix() * fwd);
}

glm::vec3 Transform::getRight()
{
	glm::vec4 right(Direction::RIGHT, 1.0f);
	return glm::vec3(getRotationMatrix() * right);
}

glm::vec3 Transform::getUp()
{
	glm::vec4 up(Direction::UP, 1.0f);
	return glm::vec3(getRotationMatrix() * up);
}





// Private functions
glm::mat4 Transform::getRotationMatrix()
{
	glm::mat4 rx = glm::rotate(glm::radians(_rotation.x), glm::vec3(1, 0, 0));
	glm::mat4 ry = glm::rotate(glm::radians(_rotation.y), glm::vec3(0, 1, 0));
	glm::mat4 rz = glm::rotate(glm::radians(_rotation.z), glm::vec3(0, 0, 1));
	return rz * ry * rx;
}

void Transform::recalculateMatrix()
{
	// Calculate different matrices
	glm::mat4 t = glm::translate(glm::vec3(_position.x, _position.y, _position.z));
	glm::mat4 s = glm::scale(glm::vec3(_scale.x, _scale.y, _scale.z));

	// finally, set matrix
	_matrix = t * getRotationMatrix() * s;
}
