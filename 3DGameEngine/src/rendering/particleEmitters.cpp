#include "rendering\particleEmitters.h"


ParticleGenerator* ParticleEmitter::getGenerator(Generators::type type)
{
	for(unsigned int i = 0; i < _generators.size(); ++i)
	{
		if(_generators[i]->getType() == type) return _generators[i];
	}
	return nullptr;
}

void ParticleEmitter::emit(float dt, ParticleData *p)
{
	const size_t maximumNewParticles = static_cast<size_t>(dt*_emitRate);
	const size_t startId = p->_aliveParticleCount;
	const size_t endId = std::min(startId + maximumNewParticles, p->_particleCount-1);

	for(auto &gen : _generators)
	{
		gen->generate(dt,p,startId,endId);
	}

	for(size_t i = startId; i < endId; ++i)
	{
		p->wake(i);
	}
}


void TestCircleEmitter::init(size_t particlePool)
{
	_emitRate=(float)particlePool*0.45f;

	posGenerator = new RoundPosGen();
	posGenerator->_center = glm::vec4(0.0,0.0,0.0,1.0);
	posGenerator->_radiusX=0.15f;
	posGenerator->_radiusY=0.15f;
	addGenerator(posGenerator);

	colGenerator = new BasicColourGen();
	colGenerator->_minStartColour	= glm::vec4( 0.0, 0.5, 0.0, 1.0 );
	colGenerator->_maxStartColour	= glm::vec4( 0.0, 1.0, 0.0, 1.0 );
	colGenerator->_minEndColour		= glm::vec4( 0.5, 0.0, 0.0, 0.0 );
	colGenerator->_maxEndColour	    = glm::vec4( 1.0, 0.0, 0.0, 0.0 );
	addGenerator(colGenerator);

	velGenerator = new BasicVelGen();
	velGenerator->_minStartVel = glm::vec4( 0.0f, 0.0f, 0.15f, 0.0f );
	velGenerator->_maxStartVel = glm::vec4( 1.0f, 0.0f, 0.45f, 0.0f );
	addGenerator(velGenerator);

	timeGenerator = new BasicTimeGen();
	timeGenerator->_minTime = 1.0;
	timeGenerator->_maxTime = 3.5;
	addGenerator(timeGenerator);
}

void ParticleEmitter::clear()
{
	for(auto it: _generators)
	{
		delete it;
	}
}

