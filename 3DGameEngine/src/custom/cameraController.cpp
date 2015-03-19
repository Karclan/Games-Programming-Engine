#include "custom\cameraController.h"

void CameraController::initialize()
{
	addEventListener(EventType::UPDATE);
	std::cout << "HELLO VORLD!";

	
	// Get Transform
	SPtr_Component transformComp = getComponent(ComponentType::TRANSFORM); // my transform
	_cameraTrans = std::static_pointer_cast<Transform>(transformComp); // cache transform so it can be manipulated in update
	

	// Get Player's Transform
	SPtr_GameObject playerObj = findGameObject("Robot");
	if(playerObj == nullptr) std::cout << "\n\n*********************\nCouldn't find player!\n\n";
	else
	{
		transformComp =  playerObj->getComponent(ComponentType::TRANSFORM);
		_playerTransform = std::static_pointer_cast<Transform>(transformComp);
	}
}

void CameraController::update(float t)
{
	// Update camera
	_cameraTrans->setPosition(_playerTransform->getPosition());
	_cameraTrans->setRotation(_playerTransform->getRotation());
	_cameraTrans->rotate(-20, glm::vec3(1, 0, 0), Transform::OBJECT_SPACE);
	_cameraTrans->translate(-_cameraTrans->getForward() * 6.0f);
}