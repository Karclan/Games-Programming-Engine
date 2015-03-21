#include "rendering\particle.h"

namespace Particles
{
	void ParticleData::generate(size_t maxSize)
	{
		_particleCount = maxSize;
		_aliveParticleCount = 0;

		_particlePositions.reset	(new glm::vec4[maxSize]);
		_particleVelocities.reset	(new glm::vec4[maxSize]);
		_particleAccelerations.reset(new glm::vec4[maxSize]);
		_particleStartColours.reset	(new glm::vec4[maxSize]);
		_particleEndColours.reset	(new glm::vec4[maxSize]);
		_particleColours.reset		(new glm::vec4[maxSize]);
		_particleLifeSpans.reset	(new glm::vec4[maxSize]);

		_particleAliveFlags.reset   (new bool[maxSize]);
	}

	void ParticleData::kill(size_t id)
	{
		if(_aliveParticleCount>0)
		{
			_particleAliveFlags[id]=false;
			swapData(id,_aliveParticleCount-1);
			_aliveParticleCount--;
		}
	}

	void ParticleData::wake(size_t id)
	{
		if(_aliveParticleCount < _particleCount)
		{
			_particleAliveFlags[id]=true;
			swapData(id,_aliveParticleCount);
			_aliveParticleCount++;
		}
	}

	void ParticleData::swapData(size_t a, size_t b)
	{
		std::swap(_particlePositions    [a],_particlePositions    [b]);
		std::swap(_particleVelocities   [a],_particleVelocities   [b]);
		std::swap(_particleAccelerations[a],_particleAccelerations[b]);
		std::swap(_particleStartColours [a],_particleStartColours [b]);
		std::swap(_particleEndColours   [a],_particleEndColours   [b]);
		std::swap(_particleColours      [a],_particleColours      [b]);
		std::swap(_particleLifeSpans    [a],_particleLifeSpans    [b]);
		std::swap(_particleAliveFlags   [a],_particleAliveFlags   [b]);
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
	
	ParticleSystem::ParticleSystem(size_t maxCount)
	{
		_particleCount = maxCount;
		_particles.generate(maxCount);
		_aliveParticles.generate(maxCount);

		for(size_t i = 0; i < maxCount; ++i)
		{
			_particles._particleAliveFlags[i]=false;
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
	void ParticleSystem::reset()
	{
		_particles._aliveParticleCount=0;
	}

	//size_t ParticleSystem::computeMemoryUsage(const ParticleSystem &p)
	//{
	//	//return 2 * ParticleData::computeMemoryUsage(p._particles);
	//}
}