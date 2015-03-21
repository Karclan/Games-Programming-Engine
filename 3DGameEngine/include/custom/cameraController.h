#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <glm\gtx\transform.hpp>

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
	void lateUpdate(float t); //!< Late Update function, so camera moves after all else

private:
	SPtr_Transform _cameraTrans; //!< Camera's transform component
	SPtr_Transform _playerTransform; //!< Robot's transform

};







#endif