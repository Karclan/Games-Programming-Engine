#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

#include "behaviour\behaviour.h"
#include "physics\physicsBody.h"

class EnemyBullet : public Behaviour
{
public:
	EnemyBullet(){};
	~EnemyBullet(){};
protected:
	void initialize();
	void onActive();
	void update(float t); //!< Update function
private:
	SPtr_PhysBody _physBody; //!< Bullets's physics body component
	float _timeToDie;
};
#endif