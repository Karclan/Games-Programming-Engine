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
	_physBody->setMass(5);
	_physBody->setDrag(1.2f);

	// Populate bullets vector
	createBullets(10);

	_accelSpeed = 20;
	_turnSpeed = 120;
	_turn = 0;
	_jumpStrength = 15;

	_turretOffset = glm::vec3(0, 0.65f, 0);
	_turretRotation = 0;
	_turretTurnSpeed = 180;

	_fire=false;
	_fireRate=0.3f;
	_timer=0.f;
}

void PlayerController::update(float t)
{
	// Translate input to axes (fwd/back and left/right)
	float axisX = 0;
	if(Input::getKeyHeld(sf::Keyboard::A)	  || Input::getJoystickAxisPosition(0,sf::Joystick::X)<=-50.f) axisX = -1;
	else if(Input::getKeyHeld(sf::Keyboard::D)|| Input::getJoystickAxisPosition(0,sf::Joystick::X)>=50.f) axisX =  1;

	float axisZ = 0;
	if(Input::getKeyHeld(sf::Keyboard::W) || Input::getJoystickAxisPosition(0,sf::Joystick::Y)<=-50.f) axisZ = 1;
	else if(Input::getKeyHeld(sf::Keyboard::S)|| Input::getJoystickAxisPosition(0,sf::Joystick::Y)>=50.f) axisZ = -1;


	// Calculate move vector based on fwd/back input axis
	_move  = _transform->getForward() * axisZ * _accelSpeed;
	
	// Set angular velocity (turn) based on left/right axis
	_turn = -axisX * _turnSpeed;

	if(Input::getJoystickAxisPosition(0,sf::Joystick::U)>=10.f||Input::getJoystickAxisPosition(0,sf::Joystick::U)<=-10.f)
	{
	_turretRotation -= t * Input::getJoystickAxisPosition(0,sf::Joystick::U);
	}
	// Set Turret Rotation
	if(Input::getKeyHeld(sf::Keyboard::E))// && _turretRotation > -60)
		_turretRotation -= _turretTurnSpeed * t;
	else if(Input::getKeyHeld(sf::Keyboard::Q))// && _turretRotation < 60)
		_turretRotation += _turretTurnSpeed * t;

	_turretTransform->setRotation(glm::rotate(_transform->getRotation(), glm::radians(_turretRotation), glm::vec3(0, 1, 0)));

	// Jump
	if(_physBody->isGrounded()) _jumpsLeft = 2;
	if((Input::getKeyPressed(sf::Keyboard::K) || Input::getKeyPressed(sf::Keyboard::Space) || Input::getJoystickButtonPressed(0,JoystickButtons::LB))  && _jumpsLeft > 1) 
	{
		glm::vec3 v = _physBody->getVelocity();
		v.y = _jumpStrength;
		_physBody->setVelocity(v);
		_jumpsLeft --;
	}

	

	// Shoot
	if(Input::getKeyPressed(sf::Keyboard::L) || Input::getKeyPressed(sf::Keyboard::RShift)||Input::getJoystickButtonPressed(0,JoystickButtons::RB)) 
	{
		//shootBullet();
		_fire=true;
	}

	if(_fire)
	{
		if(_timer>=_fireRate)
		{
			shootBullet();
			_timer=0.f;
			_fire=false;
		}
	}
	_timer+=t;
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


// Populate bullet vector
void PlayerController::createBullets(int maxBullets)
{
	for(int i = 0; i < maxBullets; ++i)
	{
		SPtr_GameObject bullet(new GameObject(0, "PBullet"));

		SPtr_Transform bulletTransform(new Transform());
		bulletTransform->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
		bullet->addComponent(bulletTransform);

		SPtr_ModelRend bulletRenderer(new ModelRenderer());
		bulletRenderer->setMesh(Assets::getMesh("bullet.obj"));
		bulletRenderer->setMaterial(Assets::getShader("advanced"), Assets::getTexture("bullet.png"), nullptr, nullptr, nullptr, glm::vec2(1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 255);
		bullet->addComponent(bulletRenderer);

		/*
		SPtr_SphereCol bulletCollider(new SphereCollider());
		bulletCollider->setRadius(0.25f);
		bullet->addComponent(bulletCollider);
		*/
		SPtr_BoxCol bulletCollider(new BoxCollider());
		bullet->addComponent(bulletCollider);

		SPtr_PhysBody bulletPhysBody(new PhysicsBody());
		bulletPhysBody->setGravity(0);
		bulletPhysBody->setDrag(0);
		bulletPhysBody->setMass(0.5f);
		bullet->addComponent(bulletPhysBody);

		SPtr_Custom bulletBehave(new Custom());
		bulletBehave->setBehaviour("PlayerBullet");
		bullet->addComponent(bulletBehave);
				

		// Now send to main system
		bullet->setActive(false);
		_bullets.push_back(bullet);
		addNewGameObject(bullet);
	}

}



void PlayerController::shootBullet()
{
	for(int i = 0; i < _bullets.size(); ++i)
	{
		if(!_bullets[i]->isActive())
		{
			SPtr_Transform bulletTransform = std::static_pointer_cast<Transform>(_bullets[i]->getComponent(ComponentType::TRANSFORM));
			SPtr_PhysBody bulletPhysBody = std::static_pointer_cast<PhysicsBody>(_bullets[i]->getComponent(ComponentType::PHY_BODY));
			bulletTransform->setPosition(_transform->getPosition() + (_turretTransform->getForward() * 0.2f) + glm::vec3(0, 0.8f, 0));
			bulletTransform->setRotation(_turretTransform->getRotation());
			bulletPhysBody->setVelocity(bulletTransform->getForward() * 50.0f);
			_bullets[i]->setActive(true);
			return;
		}
	}

}