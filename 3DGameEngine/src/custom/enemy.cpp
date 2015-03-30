#include "custom\enemy.h"



void Enemy::initialize()
{
	addEventListener(EventType::FIXED_UPDATE);

	_transform = std::static_pointer_cast<Transform>(getComponent(ComponentType::TRANSFORM));
	_physBody = std::static_pointer_cast<PhysicsBody>(getComponent(ComponentType::PHY_BODY));

	SPtr_GameObject player = findGameObject("Robot");
	_playerTrans = std::static_pointer_cast<Transform>(player->getComponent(ComponentType::TRANSFORM));
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
		
		if(_health <= 0) explode();
	}
}


void Enemy::explode()
{
	setActive(false);
}