#include "rendering\particleData.h"

void ParticleData::generate(size_t maxSize)
{
	_particleCount = maxSize;
	_aliveParticleCount = maxSize;

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
