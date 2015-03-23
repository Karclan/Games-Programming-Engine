#ifndef PARTICLEEMITTERS_H
#define PARTICLEEMITTERS_H

#include <memory>

#include "rendering\particle.h"
#include "rendering\particleGenerators.h"

class TestCircleEmitter : public ParticleEmitter
{
public:
	TestCircleEmitter(size_t particlePool);

	SP_RoundPosGen		posGenerator;
	SP_BasicColourGen	colGenerator;
	SP_BasicVelGen		velGenerator;
	SP_BasicTimeGen		timeGenerator;
};

typedef std::shared_ptr<TestCircleEmitter> SP_CircleEmitter;

#endif