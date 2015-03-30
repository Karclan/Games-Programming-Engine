#include "rendering\particleEmitters.h"


SP_ParticleGenerator ParticleEmitter::getGenerator(Generators::type type)
{
	
	for(unsigned int i = 0; i < _generators.size(); ++i)
	{
		if(_generators[i]->getType() == type) return _generators[i];
	}
	return SP_ParticleGenerator();
}

ParticleEmitter::ParticleEmitter()
{
	_emitRate=0.0f;
	_emitScalar=0.0f;

	std::cout << "Particle Emitter Start\n";
}

ParticleEmitter::~ParticleEmitter()
{
	std::cout << "Particle Emitter End\n";
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
void ParticleEmitter::init(size_t particlePool,float emitScalar)
{
	_emitScalar=emitScalar;
	init(particlePool);
}
void ParticleEmitter::init(size_t particlePool)
{
	_emitRate=(float)particlePool*_emitScalar;
}
void ParticleEmitter::addGenerator(SP_ParticleGenerator gen)
{
	_generators.push_back(gen);
}
void ParticleEmitter::clear()
{
	for(auto it: _generators)
	{
		it.reset();
	}
}

//void CircleEmitter::init(size_t particlePool)
//{
//	_emitRate=(float)particlePool*0.45f;
//
//	posGenerator.reset(new RoundPosGen());
//	posGenerator->setCenter(glm::vec4(0.0,0.0,0.0,1.0));
//	posGenerator->setXRadius(0.15f);
//	posGenerator->setYRadius(0.15f);
//	addGenerator(posGenerator);
//
//	colGenerator.reset(new BasicColourGen());
//	colGenerator->setMinStartColour(glm::vec4( 0.0, 0.5, 0.0, 1.0 ));
//	colGenerator->setMaxStartColour(glm::vec4( 0.0, 1.0, 0.0, 1.0 ));
//	colGenerator->setMinEndColour(glm::vec4( 0.5, 0.0, 0.0, 0.0 ));
//	colGenerator->setMaxEndColour(glm::vec4( 1.0, 0.0, 0.0, 0.0 ));
//	addGenerator(colGenerator);
//
//	velGenerator.reset(new BasicVelGen());
//	velGenerator->setMinStartVel(glm::vec4( 0.0f, 0.0f, 0.15f, 0.0f ));
//	velGenerator->setMaxStartVel(glm::vec4( 1.0f, 0.0f, 0.45f, 0.0f ));
//	addGenerator(velGenerator);
//
//	timeGenerator.reset(new BasicTimeGen());
//	timeGenerator->setMinTime(1.0f);
//	timeGenerator->setMaxTime(3.5f);
//	addGenerator(timeGenerator);
//}
