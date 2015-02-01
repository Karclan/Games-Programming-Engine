#include "editor\editorCamera.h"

EditorCamera::EditorCamera()
{

}

void EditorCamera::init()
{
	_transform = SPtr_Transform(new Transform());
	_transform->setPosition(glm::vec3(0, 3, 10));
	_transform->setEulerAngles(glm::vec3(-15, 0, 0));
	_camera = SPtr_Camera(new Camera());
	_camera->linkDependency(_transform);
	_targetDist = 10.0f;
}

void EditorCamera::update(float t)
{
	// ROTATE CAMERA BY HOLDING LEFT MOUSE AND LEFT ALT BUTTON
	if(Input::getKeyHeld(sf::Keyboard::LAlt) && Input::getMouseHeld(sf::Mouse::Left))
	{
		sf::Vector2f mouseDelta = -Input::getMouseDelta();
		if(mouseDelta != sf::Vector2f(0, 0))
		{
			// By translating forward by zoom before and minus that after rotation we rotate around point in front of camera
			// This could be better implemented with "rotateAround" and "lookAt" not yet implemented in Transform class.
			_transform->translate(_transform->getForward() * _targetDist);
			_transform->rotate(glm::vec3(mouseDelta.y, mouseDelta.x, 0) * (t * 100));
			_transform->translate(-_transform->getForward() * _targetDist);
		}
	}
	
	// ZOOM WITH MOUSE WHEEL
	float wheel = Input::getScrollWheelDelta() * 20;
	if(wheel != 0)
	{
		_transform->translate(_transform->getForward() * wheel);
	}

	// PAN CAMERA WITH MIDDLE MOUSE
	if(Input::getMouseHeld(sf::Mouse::Middle))
	{
		sf::Vector2f mouseMove = Input::getMouseDelta() * (t * 4);
		if(mouseMove != sf::Vector2f(0, 0))
		{
			_transform->translate(_transform->getUp() * mouseMove.y);
			_transform->translate(-_transform->getRight() * mouseMove.x);
		}
	}

}