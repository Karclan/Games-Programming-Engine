#include "behaviour\rotatingObject.h"
#include <iostream>

void RotatingObject::initialize()
{
	// Add event listeners
	addEventListener(EventType::FIXED_UPDATE);

	// Cache transform and set up an inital scale and rotation
	SPtr_Component comp = getComponent(ComponentType::TRANSFORM);
	_transform = std::static_pointer_cast<Transform>(comp); // cache transform so it can be manipulated in update
}

void RotatingObject::fixedUpdate(float t)
{
	_transform->rotate(glm::vec3(0, 0, 5));
}






// MANUAL ROTATER

void ManualRotater::initialize()
{
	// Add event listeners
	addEventListener(EventType::FIXED_UPDATE);
	addEventListener(EventType::UPDATE);

	// Cache transform and set up an inital scale and rotation
	SPtr_Component comp = getComponent(ComponentType::TRANSFORM);
	_transform = std::static_pointer_cast<Transform>(comp); // cache transform so it can be manipulated in update
	///_transform->setPosition(glm::vec3(0, 0, 0));
}

void ManualRotater::update(float t)
{
	_axisH = 0;
	if(Input::getKeyHeld(sf::Keyboard::A)) _axisH = -1;
	else if(Input::getKeyHeld(sf::Keyboard::D)) _axisH = 1;

	_axisV = 0;
	if(Input::getKeyHeld(sf::Keyboard::W)) _axisV = 1;
	else if(Input::getKeyHeld(sf::Keyboard::S)) _axisV = -1;
}

void ManualRotater::fixedUpdate(float t)
{
	_transform->rotate(glm::vec3(_axisV, _axisH, 0) * (t * 30));
}
