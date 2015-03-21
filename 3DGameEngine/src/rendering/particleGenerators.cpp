#include "rendering\particleGenerators.h"

namespace Particles
{
	namespace Generators
	{
		void BoxPosGen::generate(float dt, ParticleData *p, size_t startId, size_t endId)
		{
			glm::vec4 posMin( _position.x - _maxStartPositionOffset.x, _position.y - _maxStartPositionOffset.y, _position.z - _maxStartPositionOffset.z, 1.0 );
			glm::vec4 posMax( _position.x + _maxStartPositionOffset.x, _position.y + _maxStartPositionOffset.y, _position.z + _maxStartPositionOffset.z, 1.0 );
 
			for (size_t i = startId; i < endId; ++i)
			{
				p->_particlePositions[i] = glm::linearRand(posMin, posMax);
			}
		}
 
		void RoundPosGen::generate(float dt, ParticleData *p, size_t startId, size_t endId)
		{
			for (size_t i = startId; i < endId; ++i)
			{
				double ang = glm::linearRand(0.0, M_PI*2.0);
				p->_particlePositions[i] = _center + glm::vec4(_radiusX*sin(ang), _radiusY*cos(ang), 0.0, 1.0);
			}
		}
 
		void BasicColourGen::generate(float dt, ParticleData *p, size_t startId, size_t endId)
		{
			for (size_t i = startId; i < endId; ++i)
			{
				p->_particleStartColours[i] = glm::linearRand(_minStartColour, _maxStartColour);
				p->_particleEndColours  [i]	= glm::linearRand(_minEndColour, _maxEndColour);
			}
		}
 
		void BasicVelGen::generate(float dt, ParticleData *p, size_t startId, size_t endId)
		{
			for (size_t i = startId; i < endId; ++i)
			{
				p->_particleVelocities[i] = glm::linearRand(_minStartVel, _maxStartVel);
			}
		}
 
		void SphereVelGen::generate(float dt, ParticleData *p, size_t startId, size_t endId)
		{
			float phi, theta, v, r;
			for (size_t i = startId; i < endId; ++i)
			{

				phi = glm::linearRand(-M_PI, M_PI);
				theta = glm::linearRand(-M_PI, M_PI);
				v = glm::linearRand(_minVelocity, _maxVelocity);
 
				r = v*sinf(phi);
				p->_particleVelocities[i].z = v*cosf(phi);
				p->_particleVelocities[i].x = r*cosf(theta);
				p->_particleVelocities[i].y = r*sinf(theta);
			}
		}
 
		void BasicTimeGen::generate(float dt, ParticleData *p, size_t startId, size_t endId)
		{
			for (size_t i = startId; i < endId; ++i)
			{
				p->_particleLifeSpans[i].x = p->_particleLifeSpans[i].y = glm::linearRand(_minTime, _maxTime);
				p->_particleLifeSpans[i].z = (float)0.0;
				p->_particleLifeSpans[i].w = (float)1.0 / p->_particleLifeSpans[i].x;
			}
		}
	}
}