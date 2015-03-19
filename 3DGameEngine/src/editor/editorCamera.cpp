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


void EditorCamera::setTarget(glm::vec3 target)
{
	if(!_transform) return;
	_transform->setPosition(target);
	_transform->translate(_transform->getForward() * -_targetDist);
	_targetPos = target;
}


void EditorCamera::update(float t)
{
	_targetDist = glm::distance(_transform->getPosition(), _targetPos);

	// ROTATE CAMERA BY HOLDING LEFT MOUSE AND LEFT ALT BUTTON
	if(Input::getKeyHeld(sf::Keyboard::LAlt) && Input::getMouseHeld(sf::Mouse::Left))
	{
		sf::Vector2f mouseDelta = -Input::getMouseDelta();
		if(mouseDelta != sf::Vector2f(0, 0))
		{
			_transform->rotateAround(t * mouseDelta.y * 100, _transform->getRight(), _targetPos); // up and down. Note relative right
			_transform->rotateAround(t * mouseDelta.x * 100, glm::vec3(0, 1, 0), _targetPos); // left and right
			
		}
	}

	if(Input::getKeyPressed(sf::Keyboard::F))
	{
		//_transform->lookAt(_targetPos);
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