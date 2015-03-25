#ifndef PARTICLE_GENERATORS_H
#define PARTICLE_GENERATORS_H

#include <glm/common.hpp>
#include <glm/gtc/random.hpp>

#include "rendering\particleData.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Generators
{
	enum type{BOXPOSGEN, ROUNDPOSGEN, BASICCOLOURGEN, BASICVELGEN, SPHEREVELGEN, BASICTIMEGEN, GENERATORS};
}

class ParticleGenerator
{
public:
	ParticleGenerator(){}
	virtual ~ParticleGenerator(){}
	virtual void generate(float dt, ParticleData *p, size_t startId, size_t endId)=0;
	virtual Generators::type getType()=0;
};

typedef std::shared_ptr<ParticleGenerator> SP_ParticleGenerator;

class BoxPosGen :public ParticleGenerator
{
public:
	glm::vec4 _position;
	glm::vec4 _maxStartPositionOffset;

	BoxPosGen(){_position=glm::vec4(0.0); _maxStartPositionOffset = glm::vec4(0.0);}

	virtual void generate(float t, ParticleData *p, size_t startId, size_t endId);
	Generators::type getType(){return Generators::BOXPOSGEN;}
};

typedef std::shared_ptr<BoxPosGen> SP_BoxPosGen;

class RoundPosGen : public ParticleGenerator
{
public:
	glm::vec4 _center;
	float _radiusX;
	float _radiusY;

	RoundPosGen(){_center=glm::vec4(0.f);_radiusX=0.f;_radiusY=0.f;}

	RoundPosGen(const glm::vec4 &center, float radX, float radY){_center = center, _radiusX = radX, _radiusY = radY;}

	virtual void generate(float t, ParticleData *p, size_t startId, size_t endId);
	Generators::type getType(){return Generators::ROUNDPOSGEN;}
};

typedef std::shared_ptr<RoundPosGen> SP_RoundPosGen;

class BasicColourGen : public ParticleGenerator
{
public:
	glm::vec4 _minStartColour;
	glm::vec4 _maxStartColour;
	glm::vec4 _minEndColour;
	glm::vec4 _maxEndColour;

	BasicColourGen(){_minStartColour=glm::vec4(0.f); _maxStartColour=glm::vec4(0.f); _minEndColour=glm::vec4(0.f); _maxEndColour=glm::vec4(0.f);}

	virtual void generate(float t, ParticleData *p, size_t startId, size_t endId);
	Generators::type getType(){return Generators::BASICCOLOURGEN;}
};

typedef std::shared_ptr<BasicColourGen> SP_BasicColourGen;

class BasicVelGen : public ParticleGenerator
{
public:
	glm::vec4 _minStartVel;
	glm::vec4 _maxStartVel;

	BasicVelGen(){_minStartVel=glm::vec4(0.f); _maxStartVel=glm::vec4(0.f);}

	virtual void generate(float t, ParticleData *p, size_t startId, size_t endId);
	Generators::type getType(){return Generators::BASICVELGEN;}
};

typedef std::shared_ptr<BasicVelGen> SP_BasicVelGen;

class SphereVelGen : public ParticleGenerator
{
public:
	float _minVelocity;
	float _maxVelocity;

	SphereVelGen(){_minVelocity=0.f;_maxVelocity=0.f;}

	virtual void generate(float t, ParticleData *p, size_t startId, size_t endId);
	Generators::type getType(){return Generators::SPHEREVELGEN;}
};

typedef std::shared_ptr<SphereVelGen> SP_SphereVelGen;

class BasicTimeGen : public ParticleGenerator
{
public:
	float _minTime;
	float _maxTime;

	BasicTimeGen(){_minTime=0.f;_maxTime=0.f;}

	virtual void generate(float t, ParticleData *p, size_t startId, size_t endId);
	Generators::type getType(){return Generators::BASICTIMEGEN;}
};

typedef std::shared_ptr<BasicTimeGen> SP_BasicTimeGen;

#endif