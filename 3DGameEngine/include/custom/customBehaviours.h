#ifndef CUSTOM_BEHAVIOURS
#define CUSTOM_BEHAVIOURS

#include <memory>

#include "behaviour\playerController.h"
#include "behaviour\rotatingObject.h"


class CustomBehaviours
{
public:
	static  SPtr_Behaviour getBehaviour(std::string name)
	{
		// Define custom behaviour names and returns here
		if(name == "Player") return SPtr_Behaviour(new PlayerController());
		if(name == "Rot") return SPtr_Behaviour(new RotatingObject());
		if(name == "RotMan") return SPtr_Behaviour(new ManualRotater());
		return SPtr_Behaviour(nullptr);
	}



	

};






#endif