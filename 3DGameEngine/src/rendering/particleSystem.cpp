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
	for(auto it: _updaters)
	{
		it=nullptr;
	}
	_emitter=nullptr;
}

void ParticleSystem::update(float dt)
{
	_emitter->emit(dt,&_particles);
	
	for(size_t i = 0; i < _particleCount; ++i)
	{
		_particles._particleAccelerations[i]=glm::vec4(0.0f);
	}
	for(auto & updaters : _updaters)
	{
		updaters->update(dt,&_particles);
	}
}

void ParticleSystem::addUpdater(SP_ParticleUpdater updater)
{
	_updaters.push_back(updater);
}

void ParticleSystem::addEmitter(SP_ParticleEmitter emitter)
{
	_emitter = emitter;
}

SP_ParticleEmitter ParticleSystem::getEmitter()
{
	if(_emitter!=nullptr)return _emitter;

	return SP_ParticleEmitter();
}

SP_ParticleUpdater ParticleSystem::getUpdater(Updaters::type type)
{
	for(unsigned int i = 0; i < _updaters.size(); ++i)
	{
		if(_updaters[i]->getType() == type) return _updaters[i];
	}
	return SP_ParticleUpdater();
}

void ParticleSystem::reset()
{
	_particles._aliveParticleCount=0;
}
