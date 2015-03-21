#ifndef PARTICLE_GENERATORS_H
#define PARTICLE_GENERATORS_H

#include <glm/common.hpp>
#include <glm/gtc/random.hpp>

#include "rendering\particle.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Particles
{
	namespace Generators
	{
		class BoxPosGen :public ParticleGenerator
		{
		public:
			glm::vec4 _position;
			glm::vec4 _maxStartPositionOffset;

			BoxPosGen(){_position=glm::vec4(0.0); _maxStartPositionOffset = glm::vec4(0.0);}

			virtual void generate(float t, ParticleData *p, size_t startId, size_t endId) override;
		};

		class RoundPosGen : public ParticleGenerator
		{
		public:
			glm::vec4 _center;
			float _radiusX;
			float _radiusY;

			RoundPosGen(){_center=glm::vec4(0.f);_radiusX=0.f;_radiusY=0.f;}

			RoundPosGen(const glm::vec4 &center, float radX, float radY)
				: _center(center) , _radiusX(radX), _radiusY(radY){}

			virtual void generate(float t, ParticleData *p, size_t startId, size_t endId) override;
		};

		class BasicColourGen : public ParticleGenerator
		{
		public:
			glm::vec4 _minStartColour;
			glm::vec4 _maxStartColour;
			glm::vec4 _minEndColour;
			glm::vec4 _maxEndColour;

			BasicColourGen(){_minStartColour=glm::vec4(0.f); _maxStartColour=glm::vec4(0.f); _minEndColour=glm::vec4(0.f); _maxEndColour=glm::vec4(0.f);}

			virtual void generate(float t, ParticleData *p, size_t startId, size_t endId) override;
		};

		class BasicVelGen : public ParticleGenerator
		{
		public:
			glm::vec4 _minStartVel;
			glm::vec4 _maxStartVel;

			BasicVelGen(){_minStartVel=glm::vec4(0.f); _maxStartVel=glm::vec4(0.f);}

			virtual void generate(float t, ParticleData *p, size_t startId, size_t endId) override;
		};

		class SphereVelGen : public ParticleGenerator
		{
		public:
			float _minVelocity;
			float _maxVelocity;

			SphereVelGen(){_minVelocity=0.f;_maxVelocity=0.f;}

			virtual void generate(float t, ParticleData *p, size_t startId, size_t endId) override;
		};

		class BasicTimeGen : public ParticleGenerator
		{
		public:
			float _minTime;
			float _maxTime;

			BasicTimeGen(){_minTime=0.f;_maxTime=0.f;}

			virtual void generate(float t, ParticleData *p, size_t startId, size_t endId) override;
		};
	}
}
#endif