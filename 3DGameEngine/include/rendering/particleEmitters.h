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
	std::vector<ParticleGenerator*> _generators;
	float _emitRate;

public:
	ParticleEmitter(){_emitRate=0.0f;}
	virtual ~ParticleEmitter(){}

	virtual Emitters::type getType()=0;

	virtual void emit(float t, ParticleData *p);

	virtual void init(size_t particlePool)=0;
	virtual void clear();

	void addGenerator(ParticleGenerator* gen){ _generators.push_back(gen);}
	ParticleGenerator* getGenerator(Generators::type type);

};

class TestCircleEmitter : public ParticleEmitter
{
public:
	TestCircleEmitter(){};
	TestCircleEmitter(size_t particlePool){_poolSize=particlePool;}

	Emitters::type getType(){return Emitters::CIRCLEEMITTER;}

	void init(size_t particlePool);
	size_t _poolSize;
	RoundPosGen*		posGenerator;
	BasicColourGen*	    colGenerator;
	BasicVelGen*		velGenerator;
	BasicTimeGen*		timeGenerator;
};

#endif