#include "core\transform.h"
#include <iostream>

Transform::Transform()
{
	_position = glm::vec3(0, 0, 0);
	_rotation = quatFromEuler(glm::vec3(0, 0, 0));
	_scale = glm::vec3(1, 1, 1);
}

Transform::~Transform()
{
	std::cout << "TRANSFORM DESTORYED!\n";
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

void Transform::setRotation(glm::quat rotation) 
{ 
	_rotation = rotation;
}

void Transform::setEulerAngles(glm::vec3 ang)
{
	_rotation = quatFromEuler(ang);
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

glm::quat Transform::getRotation() 
{ 
	return _rotation;
};

glm::vec3 Transform::getEulerAngles()
{
	return glm::eulerAngles(_rotation);
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

// Note this rotates around a relative axis, not in world coordinates
void Transform::rotate(float angle, glm::vec3 axis, Space relativeTo)
{
	switch (relativeTo)
	{
	case Transform::WORLD_SPACE:
		_rotation = glm::normalize(glm::angleAxis(glm::radians(angle), axis)) * _rotation;
		break;
	case Transform::OBJECT_SPACE:
		_rotation = glm::rotate(_rotation, glm::radians(angle), axis); // works for relative axis
		break;
	default:
		break;
	}
	
	recalculateMatrix();
}

// linear interpolation to target
void Transform::lerpPosition(glm::vec3 target, float t)
{
	if(t < 0) t = 0;
	if(t > 1) t = 1;
	_position = _position + (target - _position) * t;
	recalculateMatrix();
}

void Transform::rotateAround(float angle, glm::vec3 worldAxis, glm::vec3 point)
{
	glm::vec3 newPos = _position - point; // translate so point is at origin
	_rotation = glm::normalize(glm::angleAxis(glm::radians(angle), worldAxis)) * _rotation; // rotate rotation quat
	newPos = glm::normalize(glm::angleAxis(glm::radians(angle), worldAxis)) * newPos; // rotate translated pos
	_position = newPos + point; // translate back to undo initial tranlate
	recalculateMatrix();
}

void Transform::lookAt(glm::vec3 target)
{
	glm::quat newRot = glm::quat();
	glm::vec3 targetDir = glm::normalize(target - _position); // direction to target, normalized
	glm::vec3 targetXZ = glm::normalize(glm::vec3(targetDir.x, 0, targetDir.z)); // just the XZ components of direction, normailzed

	// Work out y axis rotation using atan2, similar to in 2D games
	float angY = std::atan2(-targetDir.x, -targetDir.z);
	newRot = glm::rotate(newRot, angY, glm::vec3(0, 1, 0));

	// Now use dot product to find angle between target dir and target dir with no y component, this will give x axis rotation needed
	float angX = std::acos(glm::dot(targetDir, targetXZ));
	
	// Direction of rotation depends on if target is above or below current position
	if(targetDir.y > 0)
		newRot = glm::rotate(newRot, angX, glm::vec3(1, 0, 0));
	else if(targetDir.y < 0)
		newRot = glm::rotate(newRot, -angX, glm::vec3(1, 0, 0));

	_rotation = newRot;
	recalculateMatrix();
}




// Direction functions
glm::vec3 Transform::getForward()
{
	return _rotation * Direction::FORWARD;
}

glm::vec3 Transform::getRight()
{
	return _rotation * Direction::RIGHT;
}

glm::vec3 Transform::getUp()
{
	return _rotation * Direction::UP;
}





// Private functions
glm::mat4 Transform::getRotationMatrix()
{
	return glm::toMat4(_rotation);
}

glm::quat Transform::quatFromEuler(glm::vec3 euler)
{
	glm::mat4 rx = glm::rotate(glm::radians(euler.x), glm::vec3(1, 0, 0));
	glm::mat4 ry = glm::rotate(glm::radians(euler.y), glm::vec3(0, 1, 0));
	glm::mat4 rz = glm::rotate(glm::radians(euler.z), glm::vec3(0, 0, 1));
	return glm::toQuat(rz * ry * rx);
}

void Transform::recalculateMatrix()
{
	// Calculate different matrices
	glm::mat4 t = glm::translate(glm::vec3(_position.x, _position.y, _position.z));
	glm::mat4 s = glm::scale(glm::vec3(_scale.x, _scale.y, _scale.z));

	// finally, set matrix
	_matrix = t * getRotationMatrix() * s;
}
