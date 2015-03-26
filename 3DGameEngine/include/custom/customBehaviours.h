#ifndef CUSTOM_BEHAVIOURS
#define CUSTOM_BEHAVIOURS

#include <memory>

#include "custom\playerController.h"
#include "custom\cameraController.h"


class CustomBehaviours
{
public:
	static  SPtr_Behaviour getBehaviour(std::string name)
	{
		// Define custom behaviour names and returns here
		if(name == "Player") return SPtr_Behaviour(new PlayerController());
		if(name == "Camera") return SPtr_Behaviour(new CameraController());
		return SPtr_Behaviour(nullptr);
	}



	

};






#endif