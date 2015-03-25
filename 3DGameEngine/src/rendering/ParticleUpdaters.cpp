#include "rendering\ParticleUpdaters.h"


void EulerUpdater::update(float dt, ParticleData *p)
{
	const glm::vec4 globalA( dt * _globalAcceleration.x, dt * _globalAcceleration.y, dt * _globalAcceleration.z, 0.0 );
	const float localDT = (float)dt;

	const unsigned int endId = p->_aliveParticleCount;
	for (size_t i = 0; i < endId; ++i)
		p->_particleAccelerations[i] += globalA;

	for (size_t i = 0; i < endId; ++i)
		p->_particleVelocities[i] += localDT * p->_particleAccelerations[i];

	for (size_t i = 0; i < endId; ++i)
		p->_particlePositions[i] += localDT * p->_particleVelocities[i];
}

void AttractorUpdater::update(float dt, ParticleData *p)
{
	const float localDT = (float)dt;

	const size_t endId = p->_aliveParticleCount;
	const size_t countAttractors = collectionSize();
	glm::vec4 off;
	float dist;
	size_t a;
	for (size_t i = 0; i < endId; ++i)
	{
		for (a = 0; a < countAttractors; ++a)
		{
			off.x = _attractors[a].x - p->_particlePositions[i].x;
			off.y = _attractors[a].y - p->_particlePositions[i].y;
			off.z = _attractors[a].z - p->_particlePositions[i].z;
			dist = glm::dot(off, off);

			dist = _attractors[a].w / dist;

			p->_particleAccelerations[i] += off * dist;
		}
	}
}

void BasicColourUpdater::update(float dt, ParticleData *p)
{
	const size_t endId = p->_aliveParticleCount;
	for (size_t i = 0; i < endId; ++i)
		p->_particleColours[i] = glm::mix(p->_particleStartColours[i], p->_particleEndColours[i], p->_particleLifeSpans[i].z);
}

void PositionColourUpdater::update(float dt, ParticleData *p)
{
	const size_t endId = p->_aliveParticleCount;
	float scaler, scaleg, scaleb;
	float diffr = _maxPosition.x - _minPosition.x;
	float diffg = _maxPosition.y - _minPosition.y;
	float diffb = _maxPosition.z - _minPosition.z;
	for (size_t i = 0; i < endId; ++i)
	{
		scaler = (p->_particlePositions[i].x - _minPosition.x) / diffr;
		scaleg = (p->_particlePositions[i].y - _minPosition.y) / diffg;
		scaleb = (p->_particlePositions[i].z - _minPosition.z) / diffb;
		p->_particleColours[i].r = scaler;// glm::mix(p->m_startCol[i].r, p->m_endCol[i].r, scaler);
		p->_particleColours[i].g = scaleg;// glm::mix(p->m_startCol[i].g, p->m_endCol[i].g, scaleg);
		p->_particleColours[i].b = scaleb;// glm::mix(p->m_startCol[i].b, p->m_endCol[i].b, scaleb);
		p->_particleColours[i].a = glm::mix(p->_particleStartColours[i].a, p->_particleEndColours[i].a, p->_particleLifeSpans[i].z);
	}
}

void VelocityColourUpdater::update(float dt, ParticleData *p)
{
	const size_t endId = p->_aliveParticleCount;
	float scaler, scaleg, scaleb;
	float diffr = _maxVelocity.x - _minVelocity.x;
	float diffg = _maxVelocity.y - _minVelocity.y;
	float diffb = _maxVelocity.z - _minVelocity.z;
	for (size_t i = 0; i < endId; ++i)
	{
		scaler = (p->_particleVelocities[i].x - _minVelocity.x) / diffr;
		scaleg = (p->_particleVelocities[i].y - _minVelocity.y) / diffg;
		scaleb = (p->_particleVelocities[i].z - _minVelocity.z) / diffb;
		p->_particleColours[i].r = scaler;// glm::mix(p->m_startCol[i].r, p->m_endCol[i].r, scaler);
		p->_particleColours[i].g = scaleg;// glm::mix(p->m_startCol[i].g, p->m_endCol[i].g, scaleg);
		p->_particleColours[i].b = scaleb;// glm::mix(p->m_startCol[i].b, p->m_endCol[i].b, scaleb);
		p->_particleColours[i].a = glm::mix(p->_particleStartColours[i].a, p->_particleEndColours[i].a, p->_particleLifeSpans[i].z);
	}
}

void BasicTimeUpdater::update(float dt, ParticleData *p)
{
	unsigned int endId = p->_aliveParticleCount;
	const float localDT = (float)dt;

	if (endId == 0) return;

	for (size_t i = 0; i < endId; ++i)
	{
		p->_particleLifeSpans[i].x -= localDT;
		// interpolation: from 0 (start of life) till 1 (end of life)
		p->_particleLifeSpans[i].z = (float)1.0 - (p->_particleLifeSpans[i].x*p->_particleLifeSpans[i].w); // .w is 1.0/max life time		

		if (p->_particleLifeSpans[i].x < (float)0.0)
		{
			p->kill(i);
			endId = p->_aliveParticleCount < p->_particleCount ? p->_aliveParticleCount : p->_particleCount;
		}
	}
}
