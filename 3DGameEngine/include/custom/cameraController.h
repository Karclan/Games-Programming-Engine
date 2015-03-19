#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <SFML\Window.hpp>
#include "behaviour\behaviour.h"

/*! \brief Camera Controller

	Pans the camera round to follow player

*/

class CameraController : public Behaviour
{
public:
	CameraController(){};
	~CameraController(){};

protected:
	void initialize();
	void update(float t); //!< Update function

private:
	SPtr_Transform _cameraTrans; //!< Camera's transform component
	SPtr_Transform _playerTransform; //!< Robot's transform

};







#endif