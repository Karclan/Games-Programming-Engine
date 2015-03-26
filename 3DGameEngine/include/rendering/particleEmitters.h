#ifndef PARTICLEEMITTERS_H
#define PARTICLEEMITTERS_H

#include <memory>

#include "rendering\particleData.h"
#include "rendering\particleGenerators.h"

namespace Emitters
{
	enum type{BASEEMITTER, CIRCLEEMITTER, EMITTERS};
}

class ParticleEmitter
{
protected:
	std::vector<SP_ParticleGenerator> _generators;
	float _emitRate;
	float _emitScalar;
public:
	ParticleEmitter(){_emitRate=0.0f;_emitScalar=0.0f;}

	virtual ~ParticleEmitter(){}
	virtual Emitters::type getType(){return Emitters::BASEEMITTER;}

	virtual void emit(float t, ParticleData *p);
	virtual void init(size_t particlePool);
	virtual void init(size_t particlePool,float emitScalar);
	virtual void clear();

	void addGenerator(SP_ParticleGenerator gen);
	SP_ParticleGenerator getGenerator(Generators::type type);
	
	float getEmitScalar(){return _emitScalar;}
	float getEmitRate(){return _emitRate;}
	void setEmitScalar(float scalar){_emitScalar=scalar;}
};

typedef std::shared_ptr<ParticleEmitter> SP_ParticleEmitter;

class CircleEmitter : public ParticleEmitter
{
public:
	CircleEmitter(){};

	Emitters::type getType(){return Emitters::CIRCLEEMITTER;}

	void init(size_t particlePool);
	size_t _poolSize;
	SP_RoundPosGen		posGenerator;
	SP_BasicColourGen	colGenerator;
	SP_BasicVelGen		velGenerator;
	SP_BasicTimeGen		timeGenerator;
};

typedef std::shared_ptr<CircleEmitter> SP_CircleEmitter;

#endif