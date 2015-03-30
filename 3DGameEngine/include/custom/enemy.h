#ifndef ENEMY_H
#define ENEMY_H

#include "behaviour\behaviour.h"
#include "physics\physicsBody.h"
#include "rendering\particleRenderer.h"
#include "physics\collider.h"
#include "behaviour\custom.h"
#include "custom\levelController.h";

/*! \brief Custom Enemy Class

	Handles enemy bullet logic

*/

class Enemy : public Behaviour
{
	
public:
	enum state{ IDLE, ALERT, MOVE, SHOOT, DEAD};
	Enemy(){};
	~Enemy(){};

protected:
	void initialize();
	void onActive();
	void update(float t); //!< Update function
	void fixedUpdate(float t);

	void createBullet();
	void shootBullet(); //!< blam blam!


private:
	SPtr_Transform _transform;
	SPtr_Transform _playerTrans;
	SPtr_PhysBody _physBody; //!< Enemy's physics body component

	SPtr_ParticleRend _particleRenderer;
	float _fireworkTime;
	float _fireworkTimer;
	float _allowenceTimer;
	bool  _exploded;
	bool  _exploding;
	
	float _allowenceTime;

	SP_ParticleEmitter	  _emitter;
	SP_RoundPosGen		  _posGenerator;
	SP_BasicColourGen	  _colGenerator;
	SP_BasicVelGen		  _velGenerator;
	SP_BasicTimeGen		  _timeGenerator;
						  
	SP_EulerUpdater		  _eulerUpdater;
	SP_BasicTimeUpdater	  _timeUpdater;
	SP_BasicColourUpdater _colourUpdater;



	int _health;
	state _state;
	void explode(float t);
	float _timer;

	SPtr_GameObject _bullet;
	LevelController* _levelController;
};







#endif