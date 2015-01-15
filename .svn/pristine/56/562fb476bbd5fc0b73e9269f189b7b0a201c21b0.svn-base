#include "rendering\camera.h"

/*
Camera::Camera(SPtr_Transform transform)
{
	_transform = transform;

	// A temporary test VP matrix
	// ----View Matrix
	//glm::vec3 cameraPos(0, 0, 5); // where the camera is
	//glm::vec3 targetPos(0, 0, 0); // where the camera is looking at
	//_viewMatrix = glm::lookAt(cameraPos, targetPos, glm::vec3(0, 1, 0));

	// ----Perspective Matrix
	_projectionMatrix = glm::perspective(glm::radians(35.0f), 1.0f, 0.1f, 100.0f); //fov, aspect, zNear, ZFar

}
*/

Camera::Camera()
{
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

void Camera::preRender()
{
	_viewMatrix = glm::inverse(_transform->getMatrix()); // the view matrix is the inverse of the transform
}

void Camera::render(Renderer &renderer)
{
	renderer.render(glm::value_ptr(_viewMatrix), glm::value_ptr(_projectionMatrix));
}