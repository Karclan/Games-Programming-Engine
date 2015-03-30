#include "custom\enemy.h"



void Enemy::initialize()
{
	addEventListener(EventType::UPDATE);

	_transform = std::static_pointer_cast<Transform>(getComponent(ComponentType::TRANSFORM));
	_physBody = std::static_pointer_cast<PhysicsBody>(getComponent(ComponentType::PHY_BODY));

	SPtr_GameObject player = findGameObject("Robot");
	_playerTrans = std::static_pointer_cast<Transform>(player->getComponent(ComponentType::TRANSFORM));
}


void Enemy::onActive()
{

}


void Enemy::update(float t)
{

}


void Enemy::fixedUpdate(float t)
{

}