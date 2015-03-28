#include "custom\cameraController.h"

void CameraController::initialize()
{
	addEventListener(EventType::LATE_UPDATE);

	
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

void CameraController::lateUpdate(float t)
{
	// Update camera
	//_cameraTrans->setPosition(_playerTransform->getPosition());
	
	//glm::quat desiredRot = _playerTransform->getRotation();
	//desiredRot = glm::rotate(desiredRot, glm::radians(-20.0f), glm::vec3(1, 0, 0));
	//_cameraTrans->setRotation(glm::slerp(_cameraTrans->getRotation(), desiredRot, 0.1f));
	glm::vec3 targetPos = _playerTransform->getPosition() + glm::vec3(0, 1, 0); // Target pos offset by a height (not affected by lookat)
	glm::vec3 desiredPos = targetPos - (_playerTransform->getForward() * 8.0f);
	desiredPos.y += 1.0f; // height that is affected by lookat, e.g. if this is higher it will look down on player more
	_cameraTrans->lerpPosition(desiredPos, t * 4);
	_cameraTrans->lookAt(targetPos);
	//_cameraTrans->setPosition(desiredPos);
}