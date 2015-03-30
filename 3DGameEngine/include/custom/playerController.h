#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <SFML\Window.hpp>
#include "behaviour\behaviour.h"
#include "physics\physicsBody.h"
#include "core\input.h"
#include "physics\collider.h"
#include "behaviour\custom.h"

/*! \brief Custom Player Class

	Controls the player robot

*/

class PlayerController : public Behaviour
{
public:
	PlayerController()
	{
		test = "Hello from player\n";
	};
	~PlayerController(){};

	std::string test;

	int getScore() { return _score; }
	int getLife() { return _life; }
	void addScore(int s) {_score += s;}
	void deductLife(int l) {_life -= l;}

protected:
	void initialize();
	void update(float t); //!< Update function
	void fixedUpdate(float t); //!< Update function
	void lateUpdate(float t); //!< Update function

private:
	SPtr_Transform _transform; //!< Robot's transform
	SPtr_Transform _turretTransform; //!< Turret so we can set position and rotation
	SPtr_PhysBody _physBody; //!< Robot's physics body component

	float _accelSpeed;
	float _turnSpeed;
	float _jumpStrength; //!< How much velocity in up direction is set to when you jump
	glm::vec3 _turretOffset;
	float _turretRotation;
	float _turretTurnSpeed;

	glm::vec3 _move; //!< Amount of force to add next fixed update
	float _turn; //!< Amount to turn next fixed update

	int _life;
	int _score;

	unsigned int _jumpsLeft; //!< Jumps left for double jump
	bool _jump;  //!< True when should jump next fixed frame

	float _fireRate;
	float _timer;
	bool  _fire;
	std::vector<SPtr_GameObject> _bullets; //!< List of bullet objects to shoot

	void createBullets(int maxBullets);
	void shootBullet();
};







#endif