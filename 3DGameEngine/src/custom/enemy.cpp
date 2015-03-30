#include "custom\enemy.h"



void Enemy::initialize()
{
	addEventListener(EventType::FIXED_UPDATE);

	_transform = std::static_pointer_cast<Transform>(getComponent(ComponentType::TRANSFORM));
	_physBody = std::static_pointer_cast<PhysicsBody>(getComponent(ComponentType::PHY_BODY));

	SPtr_GameObject player = findGameObject("Robot");
	_playerTrans = std::static_pointer_cast<Transform>(player->getComponent(ComponentType::TRANSFORM));
	_state=state::ALIVE;
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
	_colGenerator->setMinStartColour(glm::vec4( 0.2, 0.2, 0.2, 1.0 ));
	_colGenerator->setMaxStartColour(glm::vec4( 0.4, 0.4, 0.4, 1.0 ));
	_colGenerator->setMinEndColour	(glm::vec4( 0.1, 0.1, 0.1, 1.0 ));
	_colGenerator->setMaxEndColour	(glm::vec4( 0.4, 0.4, 0.4, 1.0 ));
	_velGenerator.reset(new BasicVelGen());
	_velGenerator->setMinStartVel	(glm::vec4( -0.45f,  5.0f,  -0.45f, 0.0f ));
	_velGenerator->setMaxStartVel	(glm::vec4( 0.45f,   5.45f,  0.45f, 0.0f ));
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
}


void Enemy::onActive()
{
	_health = 3;
}


void Enemy::update(float t)
{



}


void Enemy::fixedUpdate(float t)
{
	switch(_state)
	{
	case state::ALIVE:
		{
			glm::vec3 vecToPlayer = _playerTrans->getPosition() - _transform->getPosition();
			float sqrDist = glm::dot(vecToPlayer, vecToPlayer);

			if(sqrDist < 100)
			{
				// You are close, he should attack
				float thatsRad = std::atan2(-vecToPlayer.z, vecToPlayer.x);
				glm::vec3 rot = glm::vec3(0, glm::degrees(thatsRad) - 90, 0);
				_transform->setEulerAngles(rot);
			}

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
				_particleRenderer->setParticlePointSize(7.f);
				_state=state::DEAD;
				}
			}
			break;
		}
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