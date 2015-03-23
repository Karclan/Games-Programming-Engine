#ifndef PARTICLE_EMITTERS_H
#define PARTICLE_EMITTERS_H

#include "glm/glm.hpp"

#include "rendering\particle.h"
#include "rendering\particleGenerators.h"

class CircleEmitter : public ParticleEmitter
{
public:
	ParticleEmitters(){}

	void updateCenter(glm::vec4 newCenter);

private:

	RoundPosGen positionGenerator;

}

#endif