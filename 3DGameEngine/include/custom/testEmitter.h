#ifndef TEST_EMITTER_H
#define TEST_EMITTER_H

#include "behaviour\behaviour.h"
#include "core\input.h"
#include "rendering\particleRenderer.h"

class TestEmitter : public Behaviour
{
private:
	SPtr_ParticleRend _particleRenderer;
public:
	TestEmitter();

	void initialize();
	void update(float t);
};

#endif