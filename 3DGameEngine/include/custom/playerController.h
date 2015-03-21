#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <SFML\Window.hpp>
#include "behaviour\behaviour.h"
#include "physics\physicsBody.h"
#include "core\input.h"

/*! \brief Temporary behaviour class

	Hard-coded behaviour for controlling the player. In final version
	this would be a script file.

*/

class PlayerController : public Behaviour
{
public:
	PlayerController(){};
	~PlayerController(){};

protected:
	void initialize();
	void update(float t); //!< Update function
	void fixedUpdate(float t); //!< Update function

private:
	SPtr_Transform _transform; //!< Robot's transform
	SPtr_PhysBody _physBody; //!< Robot's physics body component
	float _accelSpeed;
	float _turnSpeed;

	glm::vec3 _move; //!< Amount of force to add next fixed update
	float _turn; //!< Amount to turn next fixed update

	// Basic jump implemented - in final version this would probably be handled by the physics subsystem to take care of collisions etc. This is just a demo
	float _jumpStrength; //!< How much velocity in up direction is set to when you jump
};







#endif