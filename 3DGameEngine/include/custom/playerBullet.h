#ifndef PLAYER_BULLET_H
#define PLAYER_BULLET_H

#include "behaviour\behaviour.h"
#include "physics\physicsBody.h"

/*! \brief Custom Bullet Class

	Handles player bullet logic

*/

class PlayerBullet : public Behaviour
{
public:
	PlayerBullet(){};
	~PlayerBullet(){};

protected:
	void initialize();
	void onActive();
	void update(float t); //!< Update function

private:
	SPtr_PhysBody _physBody; //!< Bullets's physics body component
	float _timeToDie;
};







#endif