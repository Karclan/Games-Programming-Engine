#ifndef CUSTOM_BEHAVIOURS
#define CUSTOM_BEHAVIOURS

#include <memory>

#include "custom\playerController.h"
#include "custom\cameraController.h"
#include "custom\testEmitter.h"
#include "custom\firework.h"
#include "custom\playerBullet.h"
#include "custom\enemy.h"

class CustomBehaviours
{
public:
	static  SPtr_Behaviour getBehaviour(std::string name)
	{
		// Define custom behaviour names and returns here
		if(name == "Player") return SPtr_Behaviour(new PlayerController());
		if(name == "Camera") return SPtr_Behaviour(new CameraController());
		if(name == "TestEmitter") return SPtr_Behaviour(new TestEmitter());
		if(name == "Firework")	  return SPtr_Behaviour(new Firework());
		if(name == "PlayerBullet") return SPtr_Behaviour(new PlayerBullet());
		if(name == "Enemy") return SPtr_Behaviour(new Enemy());
		return SPtr_Behaviour(nullptr);
	}

};






#endif