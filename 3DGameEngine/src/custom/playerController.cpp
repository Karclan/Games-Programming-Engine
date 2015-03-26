#include "custom\playerController.h"

void PlayerController::initialize()
{
	addEventListener(EventType::UPDATE);
	addEventListener(EventType::FIXED_UPDATE);
	addEventListener(EventType::LATE_UPDATE);

	// Get Transform
	SPtr_Component transComp = getComponent(ComponentType::TRANSFORM);
	_transform = std::static_pointer_cast<Transform>(transComp); // cache transform so it can be manipulated in update

	// Get Turret Trans
	SPtr_GameObject turret = findGameObject("PlayerTurret");
	if(turret)
		_turretTransform = std::static_pointer_cast<Transform>(turret->getComponent(ComponentType::TRANSFORM));
	else
		std::cout << "ERROR! Can't find turret!\n";

	// Get Physics Body
	SPtr_Component physComp = getComponent(ComponentType::PHY_BODY);
	_physBody = std::static_pointer_cast<PhysicsBody>(physComp);

	_accelSpeed = 20;
	_turnSpeed = 120;
	_turn = 0;
	_jumpStrength = 15;

	_turretOffset = glm::vec3(0, 0.65f, 0);
	_turretRotation = 0;
	_turretTurnSpeed = 180;
}

void PlayerController::update(float t)
{
	// Translate input to axes (fwd/back and left/right)
	float axisX = 0;
	if(Input::getKeyHeld(sf::Keyboard::A)) axisX = -1;
	else if(Input::getKeyHeld(sf::Keyboard::D)) axisX = 1;

	float axisZ = 0;
	if(Input::getKeyHeld(sf::Keyboard::W)) axisZ = 1;
	else if(Input::getKeyHeld(sf::Keyboard::S)) axisZ = -1;


	// Calculate move vector based on fwd/back input axis
	_move  = _transform->getForward() * axisZ * _accelSpeed;
	
	// Set angular velocity (turn) based on left/right axis
	_turn = -axisX * _turnSpeed;

	// Set Turret Rotation
	if(Input::getKeyHeld(sf::Keyboard::E) && _turretRotation > -70)
		_turretRotation -= _turretTurnSpeed * t;
	else if(Input::getKeyHeld(sf::Keyboard::Q) && _turretRotation < 70)
		_turretRotation += _turretTurnSpeed * t;

	_turretTransform->setRotation(glm::rotate(_transform->getRotation(), glm::radians(_turretRotation), glm::vec3(0, 1, 0)));

	// Jump
	if(Input::getKeyPressed(sf::Keyboard::K)) 
	{
		_physBody->addImpulse(glm::vec3(0, _jumpStrength, 0));
	}

	// Shoot
	if(Input::getKeyPressed(sf::Keyboard::L)) 
	{
		SPtr_GameObject bullet(new GameObject(0, "Bullet"));

		SPtr_Transform bulletTransform(new Transform());
		bulletTransform->setPosition(_transform->getPosition() + (_turretTransform->getForward() * 0.5f) + glm::vec3(0, 1, 0));
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
		bulletPhysBody->addImpulse((_turretTransform->getForward() * 20.0f) + glm::vec3(0, 10, 0));
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


void PlayerController::lateUpdate(float t)
{
	// Update turret pos. Done in late update to ensure matches player's position right before rendering
	_turretTransform->setPosition(_transform->getPosition() + _turretOffset);
}