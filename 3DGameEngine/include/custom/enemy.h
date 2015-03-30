#ifndef ENEMY_H
#define ENEMY_H

#include "behaviour\behaviour.h"
#include "physics\physicsBody.h"

/*! \brief Custom Enemy Class

	Handles enemy bullet logic

*/

class Enemy : public Behaviour
{
public:
	Enemy(){};
	~Enemy(){};

protected:
	void initialize();
	void onActive();
	void update(float t); //!< Update function
	void fixedUpdate(float t);

private:
	SPtr_Transform _transform;
	SPtr_Transform _playerTrans;
	SPtr_PhysBody _physBody; //!< Enemy's physics body component

	int _health;


	void explode();
};







#endif