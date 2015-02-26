#ifndef JILL_DANDO
#define JILL_DANDO

#include <memory>

#include "behaviour\playerController.h" // this is temporary, as no real scripts yet it needs to know EVERY hard-coded behaviour
#include "behaviour\rotatingObject.h" //


class CustomBehaviours
{
public:
	static  SPtr_Behaviour getBehaviour(std::string name)
	{
		if(name == "Player") return SPtr_Behaviour(new PlayerController());
		if(name == "Rot") return SPtr_Behaviour(new RotatingObject());
		if(name == "RotMan") return SPtr_Behaviour(new ManualRotater());
		return SPtr_Behaviour(nullptr);
	}



	

};






#endif