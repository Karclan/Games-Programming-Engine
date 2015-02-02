#include "rendering\camera.h"
#include <iostream>


Camera::Camera()
{
	// Set dependency flags so linkDependency is called with requested component
	setDepFlag(ComponentType::TRANSFORM); // requires a transform

	_transform = nullptr;
	_projectionMatrix = glm::perspective(glm::radians(35.0f), 1280.0f / 720.0f, 0.1f, 100.0f); //fov, aspect, zNear, ZFar
}

ComponentType::Type Camera::getType()
{
	return ComponentType::CAMERA;
}

bool Camera::isOnePerObject()
{
	return false;
}

void Camera::linkDependency(SPtr_Component component)
{
	// Switch through type and cast then cache the component
	switch(component->getType())
	{
	case ComponentType::TRANSFORM:
		_transform = std::static_pointer_cast<Transform>(component);
		break;
	}
}

void Camera::preRender()
{
	_viewMatrix = glm::inverse(_transform->getMatrix()); // the view matrix is the inverse of the transform
}

void Camera::render(Renderer &renderer)
{
	renderer.render(glm::value_ptr(_viewMatrix), glm::value_ptr(_projectionMatrix));
}