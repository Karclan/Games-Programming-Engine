#include "custom\enemy.h"



void Enemy::initialize()
{
	addEventListener(EventType::FIXED_UPDATE);

	_transform = std::static_pointer_cast<Transform>(getComponent(ComponentType::TRANSFORM));
	_physBody = std::static_pointer_cast<PhysicsBody>(getComponent(ComponentType::PHY_BODY));

	SPtr_GameObject player = findGameObject("Robot");
	_playerTrans = std::static_pointer_cast<Transform>(player->getComponent(ComponentType::TRANSFORM));
	_state=state::IDLE;
	_allowenceTime=1.5f;

	_fireworkTimer=0.f;
	_allowenceTimer=0.f;
	_fireworkTime=1.5f;

	_exploded = false;

	addEventListener(EventType::UPDATE);

	_particleRenderer = std::static_pointer_cast<ParticleRenderer>(getComponent(ComponentType::PARTICLE_REND));

	_posGenerator.reset(new RoundPosGen());
	_posGenerator->setCenter(glm::vec4(0.0,5.0,0.0,1.0));
	_posGenerator->setXRadius(0.10f);
	_posGenerator->setYRadius(0.10f);
	_colGenerator.reset(new BasicColourGen());
	_colGenerator->setMinStartColour(glm::vec4( 0.5, 0.5, 0.5, 1.0 ));
	_colGenerator->setMaxStartColour(glm::vec4( 1.0, 1.0, 1.0, 1.0 ));
	_colGenerator->setMinEndColour	(glm::vec4( 0.0, 0.0, 0.0, 1.0 ));
	_colGenerator->setMaxEndColour	(glm::vec4( 0.2, 0.2, 0.2, 1.0 ));
	_velGenerator.reset(new BasicVelGen());
	_velGenerator->setMinStartVel	(glm::vec4( -5.45f, -5.45f, -5.45f, 0.0f ));
	_velGenerator->setMaxStartVel	(glm::vec4(  5.45f,  5.45f,  5.45f, 0.0f ));
	_timeGenerator.reset(new BasicTimeGen());
	_timeGenerator->setMinTime(0.5f);
	_timeGenerator->setMaxTime(2.5f);

	_emitter.reset(new ParticleEmitter());
	_emitter->init(1000,1.f);
	_emitter->addGenerator(_posGenerator);
	_emitter->addGenerator(_colGenerator);
	_emitter->addGenerator(_velGenerator);
	_emitter->addGenerator(_timeGenerator);
	_particleRenderer->addEmitter(_emitter);

	_timeUpdater.reset(new BasicTimeUpdater());
	_colourUpdater.reset(new BasicColourUpdater());
	_eulerUpdater.reset(new EulerUpdater());

	_eulerUpdater->setGlobalAcc(glm::vec4(0.0,-15.0,0.0,0.0));

	_particleRenderer->clear();//clears updaters
	_particleRenderer->addUpdater(_timeUpdater);
	_particleRenderer->addUpdater(_colourUpdater);
	_particleRenderer->addUpdater(_eulerUpdater);

	_particleRenderer->generate(1000);
	_particleRenderer->setEmitFlag(false);
	_particleRenderer->resetParticles();
	_particleRenderer->setParticlePointSize(30.f);

	createBullet();

	_health = 3;
}


void Enemy::onActive()
{
	_health = 3;
	_timer = 0;

	SPtr_GameObject bobo = findGameObject("GUI");
	SPtr_Custom levelContCyst = std::static_pointer_cast<Custom>(bobo->getComponent(ComponentType::CUSTOM));
	_levelController = static_cast<LevelController*>(levelContCyst->getBehaviour().get());
}


void Enemy::update(float t)
{
	_particleRenderer->setParticlePointSize(30.f);
}


void Enemy::fixedUpdate(float t)
{
	// If bullet hits, lose health
	if(_state != DEAD)
	{
		if(_physBody->hasCollided())
		{
			std::vector<SPtr_GameObject> collidingObjs;
			getCollisions(_physBody, collidingObjs);
			for(int i = 0; i < collidingObjs.size(); ++i)
			{
				if(collidingObjs[i]->getName() == "PBullet")
				{
					_health--;
					continue;
				}
			}
			if(_health <= 0)
			{
				_levelController->incScore(10);
				_state=state::DEAD;
			}
		}
	}


	// Main state switch
	switch(_state)
	{
	case state::IDLE:
		{
			// Work out dist to player
			glm::vec3 vecToPlayer = _playerTrans->getPosition() - _transform->getPosition();
			float sqrDist = glm::dot(vecToPlayer, vecToPlayer);

			// If close attack
			if(sqrDist < 600)
			{
				_state = ALERT;
			}
			else if(sqrDist < 1000)
			{
				_timer += t;
				if(_timer > 0.3f)
				{
					_timer = 0;
					int choice = rand() %20;

					if(choice >= 6)
						_state = SHOOT;
				}
			}

			break;
		}

	case state::ALERT:
		{
			glm::vec3 vecToPlayer = _playerTrans->getPosition() - _transform->getPosition();
			float sqrDist = glm::dot(vecToPlayer, vecToPlayer);

			// Look at player
			float thatsRad = std::atan2(-vecToPlayer.z, vecToPlayer.x);
			glm::vec3 rot = glm::vec3(0, glm::degrees(thatsRad) - 90, 0);
			_transform->setEulerAngles(rot);

			// If close attack
			if(sqrDist < 50)
			{
				int choice = rand() %20;

				if(choice < 15)
					_state = MOVE;
				else
					_state = SHOOT;
			}

		}
		break;


	case state::MOVE:
		{
			_physBody->addForce(_transform->getForward() * 30.0f);
			_timer += t;

			if(_timer > 1.0f)
			{
				_timer = 0;
				_state = state::IDLE;
			}
		}
		break;

	case state::SHOOT:
		{
			_timer += t;

			if(_timer > 0.2f)
			{
				shootBullet();
				_timer = 0;
				_state = state::IDLE;
			}
		}
		break;

	case state::DEAD:
		{
			explode(t);
			break;
		}
	}
}


void Enemy::explode(float t)
{
	_fireworkTimer+=t;
	if(_fireworkTimer>=_fireworkTime)
		{
			_particleRenderer->setEmitFlag(false);
			_exploded=true;
		}
	else
		{
			_particleRenderer->setEmitFlag(true);
		}
	if(_exploded)
	{
		if(_allowenceTimer>=_allowenceTime)
		{
			setActive(false);
		}
		else
		{
			_allowenceTimer+=t;
		}
	}
}

void Enemy::shootBullet()
{
	if(!_bullet->isActive())
	{
		SPtr_Transform bulletTransform = std::static_pointer_cast<Transform>(_bullet->getComponent(ComponentType::TRANSFORM));
		SPtr_PhysBody bulletPhysBody = std::static_pointer_cast<PhysicsBody>(_bullet->getComponent(ComponentType::PHY_BODY));
		bulletTransform->setPosition(_transform->getPosition() + (_transform->getForward() * 0.2f) + glm::vec3(0, 0.8f, 0));
		bulletTransform->setRotation(_transform->getRotation());
		bulletPhysBody->setVelocity(bulletTransform->getForward() * 50.0f);
		_bullet->setActive(true);
		return;
	}

}



void Enemy::createBullet()
{
	SPtr_GameObject bullet(new GameObject(0, "EBullet"));

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
	_bullet = bullet;
	addNewGameObject(bullet);

}