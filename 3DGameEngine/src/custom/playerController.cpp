#include "custom\playerController.h"

void PlayerController::initialize()
{
	addEventListener(EventType::UPDATE);
	addEventListener(EventType::FIXED_UPDATE);

	// Get Transform
	SPtr_Component transComp = getComponent(ComponentType::TRANSFORM);
	_transform = std::static_pointer_cast<Transform>(transComp); // cache transform so it can be manipulated in update

	// Get Physics Body
	SPtr_Component physComp = getComponent(ComponentType::PHY_BODY);
	_physBody = std::static_pointer_cast<PhysicsBody>(physComp);

	_accelSpeed = 40;
	_turnSpeed = 120;
	_turn = 0;
	_jumpStrength = 15;
}

void PlayerController::update(float t)
{
	// Translate input to axes (fwd/back and left/right)
	float axisX = 0;
	if(Input::getKeyHeld(sf::Keyboard::Left)) axisX = -1;
	else if(Input::getKeyHeld(sf::Keyboard::Right)) axisX = 1;

	float axisY = 0;
	if(Input::getKeyHeld(sf::Keyboard::U)) axisY = 1;
	else if(Input::getKeyHeld(sf::Keyboard::D)) axisY = -1;

	float axisZ = 0;
	if(Input::getKeyHeld(sf::Keyboard::Up)) axisZ = 1;
	else if(Input::getKeyHeld(sf::Keyboard::Down)) axisZ = -1;


	// Calculate move vector based on fwd/back input axis
	_move  = _transform->getForward() * axisZ * _accelSpeed;
	

	// Set angular velocity (turn) based on left/right axis
	_turn = -axisX * _turnSpeed;

	// If press space, jump
	if(Input::getKeyPressed(sf::Keyboard::J)) 
	{
		_physBody->addImpulse(glm::vec3(0, _jumpStrength, 0));
	}

	// Animation
	//if(axisV == 0) _robotAnim->stop();
	//else _robotAnim->play();

	
}

void PlayerController::fixedUpdate(float t)
{
	// Update movement
	_physBody->addForce(_move);

	// Update rotation
	_transform->rotate(_turn * t, glm::vec3(0, 1, 0), Transform::WORLD_SPACE);
}