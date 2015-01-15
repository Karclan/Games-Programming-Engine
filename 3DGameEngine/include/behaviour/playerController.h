#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <SFML\Window.hpp>
#include "behaviour\behaviour.h"
#include "core\input.h"
#include "rendering\robotRenderer.h"

/*! \brief Temporary behaviour class

	Hard-coded behaviour for controlling the player. In final version
	this would be a script file.

*/

class PlayerController : public Behaviour
{
public:
	PlayerController();
	~PlayerController();

protected:
	void initialize();
	void update(float t); //!< Update function
	void fixedUpdate(float t); //!< Update function

private:
	SPtr_Transform _transform; //!< Robot's transform
	SPtr_Transform _cameraTrans; //!< Camera's transform component
	std::shared_ptr<RobotRenderer> _robotAnim; //!< Robot Renderer so we can control animations
	float _speed;
	glm::vec3 _velocity;
	float _turn;

	// Basic jump implemented - in final version this would probably be handled by the physics subsystem to take care of collisions etc. This is just a demo
	bool _jumping; //!< Is the robot jumping or not?
	float _groundHeight; //!< When the robot's middle is at this point, he's hit the ground
	float _gravitationalPull; //!< How fast he falls back to earth after jumping
	float _jumpStrength; //!< How much velocity in up direction is set to when you jump
};







#endif