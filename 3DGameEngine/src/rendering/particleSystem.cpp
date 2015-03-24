#include "rendering\particleSystem.h"

ParticleSystem::ParticleSystem(size_t maxCount)
{
	_particleCount = maxCount;
	_particles.generate(maxCount);

	for(size_t i = 0; i < maxCount; ++i)
	{
		_particles._particleAliveFlags[i]=false;
	}
}

void ParticleSystem::clear()
{
	for(auto it: _emitters)
	{
		it->clear();
		delete it;
	}
	for(auto it: _updaters)
	{
		delete it;
	}
}

void ParticleSystem::update(float dt)
{
	for(auto & emitters: _emitters)
	{
		emitters->emit(dt,&_particles);
	}
	for(size_t i = 0; i < _particleCount; ++i)
	{
		_particles._particleAccelerations[i]=glm::vec4(0.0f);
	}
	for(auto & updaters : _updaters)
	{
		updaters->update(dt,&_particles);
	}
}

ParticleEmitter* ParticleSystem::getEmitter(Emitters::type type)
{
	for(unsigned int i = 0; i < _emitters.size(); ++i)
	{
		if(_emitters[i]->getType() == type) return _emitters[i];
	}
	return nullptr;
}

ParticleUpdater* ParticleSystem::getUpdater(Updaters::type type)
{
	for(unsigned int i = 0; i < _updaters.size(); ++i)
	{
		if(_updaters[i]->getType() == type) return _updaters[i];
	}
	return nullptr;
}

void ParticleSystem::reset()
{
	_particles._aliveParticleCount=0;
}
