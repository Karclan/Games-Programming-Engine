#include "custom\playerBullet.h"


void PlayerBullet::initialize()
{
	addEventListener(EventType::UPDATE);

	_physBody = std::static_pointer_cast<PhysicsBody>(getComponent(ComponentType::PHY_BODY));
}

void PlayerBullet::onActive()
{
	_timeToDie = 1.0f;
}

void PlayerBullet::update(float t)
{
	// If collided, destroy
	if(_physBody->hasCollided())
	{
		setActive(false);
	}

	// Kill if lifespan ends
	_timeToDie -= t;
	if(_timeToDie <= 0)
	{
		setActive(false);
	}
}