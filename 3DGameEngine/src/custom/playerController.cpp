#include "custom\playerController.h"

void PlayerController::initialize()
{
	addEventListener(EventType::UPDATE);
	addEventListener(EventType::FIXED_UPDATE);

	// Get Transform
	SPtr_Component comp = getComponent(ComponentType::TRANSFORM);
	_transform = std::static_pointer_cast<Transform>(comp); // cache transform so it can be manipulated in update

	_speed = 2;
	_turn = 0;
	_jumping = false;
	_groundHeight = 0.8f;
	_gravitationalPull = 10;
	_jumpStrength = 5;
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
	glm::vec3 move  = _transform->getForward() * axisZ * _speed;
	_velocity.x = move.x;
	_velocity.y = axisY * _speed;
	_velocity.z = move.z;

	// Set angular velocity (turn) based on left/right axis
	_turn = -axisX * _speed * 60;

	// If press space, jump
	/*
	if(!_jumping && Input::getKeyPressed(sf::Keyboard::J)) 
	{
		_jumping = true;
		_velocity.y = _jumpStrength;
	}
	*/

	// Animation
	//if(axisV == 0) _robotAnim->stop();
	//else _robotAnim->play();

	
}

void PlayerController::fixedUpdate(float t)
{
	// Update position
	_transform->translate(_velocity * t);

	/*
	if(_jumping) _velocity.y -= _gravitationalPull * t; // If jumping, fall
	if(_transform->getPosition().y < _groundHeight) // if lower than ground, snap to ground and make not jumping
	{
		_jumping = false;
		_velocity.y = 0;
		glm::vec3 newPos = _transform->getPosition();
		newPos.y = _groundHeight;
		_transform->setPosition(newPos);
	}
	*/

	// Update rotation
	_transform->rotate(_turn * t, glm::vec3(0, 1, 0), Transform::WORLD_SPACE);
}