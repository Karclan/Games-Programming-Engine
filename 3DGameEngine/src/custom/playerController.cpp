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

	_accelSpeed = 20;
	_turnSpeed = 120;
	_turn = 0;
	_jumpStrength = 15;
}

void PlayerController::update(float t)
{
	// Translate input to axes (fwd/back and left/right)
	float axisX = 0;
	if(Input::getKeyHeld(sf::Keyboard::Left)||Input::getJoystickAxisPosition(0,sf::Joystick::Axis::X)<-50.f) axisX = -1;
	else if(Input::getKeyHeld(sf::Keyboard::Right)||Input::getJoystickAxisPosition(0,sf::Joystick::Axis::X)>50.f) axisX = 1;

	float axisY = 0;
	if(Input::getKeyHeld(sf::Keyboard::U)) axisY = 1;
	else if(Input::getKeyHeld(sf::Keyboard::D)) axisY = -1;

	float axisZ = 0;
	if(Input::getKeyHeld(sf::Keyboard::Up)||Input::getJoystickAxisPosition(0,sf::Joystick::Axis::Y)<-50.f) axisZ = 1;
	else if(Input::getKeyHeld(sf::Keyboard::Down)||Input::getJoystickAxisPosition(0,sf::Joystick::Axis::Y)>50.f) axisZ = -1;


	// Calculate move vector based on fwd/back input axis
	_move  = _transform->getForward() * axisZ * _accelSpeed;
	

	// Set angular velocity (turn) based on left/right axis
	_turn = -axisX * _turnSpeed;

	// If press J, jump
	if(Input::getKeyPressed(sf::Keyboard::J)) 
	{
		_physBody->addImpulse(glm::vec3(0, _jumpStrength, 0));
	}

	// If press S, shoot (I'm such a troll...)
	
	if(Input::getKeyPressed(sf::Keyboard::S)) 
	{
		SPtr_GameObject bullet(new GameObject(0, "Bullet"));

		SPtr_Transform bulletTransform(new Transform());
		bulletTransform->setPosition(_transform->getPosition() + glm::vec3(0, 1, 0));
		bulletTransform->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
		bullet->addComponent(bulletTransform);

		SPtr_ModelRend bulletRenderer(new ModelRenderer());
		bulletRenderer->setMesh(Assets::getMesh("sphere"));
		bulletRenderer->setMaterial(Assets::getShader("advanced"));
		bullet->addComponent(bulletRenderer);

		SPtr_SphereCol bulletCollider(new SphereCollider());
		bulletCollider->setRadius(0.25f);
		bullet->addComponent(bulletCollider);

		SPtr_PhysBody bulletPhysBody(new PhysicsBody());
		bulletPhysBody->addImpulse((_transform->getForward() * 20.0f) + glm::vec3(0, 10, 0));
		bullet->addComponent(bulletPhysBody);

		

		// Now send to main system
		addNewGameObject(bullet);
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