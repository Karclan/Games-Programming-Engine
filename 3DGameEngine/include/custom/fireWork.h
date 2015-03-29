#ifndef FIREWORK_H
#define FIREWORK_H

#include "behaviour\behaviour.h"
#include "core\input.h"
#include "rendering\particleRenderer.h"

class Firework : public Behaviour
{
private:
	SPtr_ParticleRend _particleRenderer;
	float _fireworkTime;
	float _time;
	bool  _explode;
	bool  _exploding;

	SP_ParticleEmitter	  _emitter;
	SP_RoundPosGen		  _posGenerator;
	SP_BasicColourGen	  _colGenerator;
	SP_BasicVelGen		  _velGenerator;
	SP_BasicTimeGen		  _timeGenerator;
						  
						  
	SP_EulerUpdater		  _eulerUpdater;
	SP_FloorUpdater		  _floorUpdater;
	SP_BasicTimeUpdater	  _timeUpdater;
	SP_BasicColourUpdater _colourUpdater;
public:
	Firework();

	void initialize();
	void update(float t);
};

#endif