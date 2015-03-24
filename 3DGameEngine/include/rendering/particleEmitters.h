#ifndef PARTICLEEMITTERS_H
#define PARTICLEEMITTERS_H

#include <memory>

#include "rendering\particleData.h"
#include "rendering\particleGenerators.h"

namespace Emitters
{
	enum type{CIRCLEEMITTER, EMITTERS};
}

class ParticleEmitter
{
protected:
	std::vector<SP_ParticleGenerator> _generators;
	float _emitRate;

public:
	ParticleEmitter(){_emitRate=0.0f;}
	virtual ~ParticleEmitter(){}

	virtual Emitters::type getType()=0;

	virtual void init(size_t particlePool)=0;
	virtual void emit(float t, ParticleData *p);

	void addGenerator(SP_ParticleGenerator gen){ _generators.push_back(gen);}
	SP_ParticleGenerator getGenerator(Generators::type type);

};

typedef std::shared_ptr<ParticleEmitter> SP_ParticleEmitter;

class TestCircleEmitter : public ParticleEmitter
{
public:
	TestCircleEmitter(){};

	Emitters::type getType(){return Emitters::CIRCLEEMITTER;}

	void init(size_t particlePool);
	SP_RoundPosGen		posGenerator;
	SP_BasicColourGen	colGenerator;
	SP_BasicVelGen		velGenerator;
	SP_BasicTimeGen		timeGenerator;
};

typedef std::shared_ptr<TestCircleEmitter> SP_CircleEmitter;

#endif