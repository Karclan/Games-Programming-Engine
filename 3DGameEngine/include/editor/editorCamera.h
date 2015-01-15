#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "rendering\camera.h"
#include "core\transform.h"
#include "core\input.h"


/*! \brief Special Game Object used only by editor to render scene in editor mode

	This special object exists outside of the main engine and is held in the editor class
	It has a camera component that is passed to the render system when the editor is not 
	in play mode, allowing the scene to be rendered through that camera. It is updated only
	when the editor is not in play mode, allowing the user to move the camera with the mouse
	to explore the scene in "edit" mode.

	Note due to problems with "lookAt" and "rotateAround" functions in Transform class (see transform.h
	description for more details) I have implemented some functionality in an unusual way! Re-coding this
	after those functions have been implemented will likely give more flexibility and be better
	optimized and easier to understand.
*/
class EditorCamera
{
public:
	EditorCamera();
	void init(); //!< Explicit call rather than in constructor because it needs to be set up after editor initalized
	void update(float t); //!< Process mouse input and alter transform accordingly
	Camera* getCamera() { return _camera.get(); }

private:
	SPtr_Transform _transform; //!< Transform for the camera
	SPtr_Camera _camera; //!< Camera
	float _targetDist; //!< Distance to target, affecting the point we rotate around etc

};

#endif