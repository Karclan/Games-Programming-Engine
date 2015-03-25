#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "rendering\particleData.h"
#include "rendering\particleEmitters.h"
#include "rendering\particleGenerators.h"
#include "rendering\ParticleUpdaters.h"

class ParticleSystem
{
public:
	ParticleData _particles;

	size_t _particleCount;

	ParticleEmitter* _emitter; 
	std::vector<ParticleUpdater*> _updaters;

public:
	ParticleSystem(size_t maxCount);
	virtual ~ParticleSystem()
	{
		delete _emitter;
		_updaters.clear();
	}
	 
	void clear();
    virtual void update(float t);
	virtual void reset();
	virtual size_t getParticleCount(){return _particles._particleCount;}

	void addEmitter(SP_ParticleEmitter emitter);
	void addUpdater(SP_ParticleUpdater updater);
	ParticleEmitter* getEmitter(Emitters::type type);
	ParticleUpdater* getUpdater(Updaters::type type);

	ParticleData *getFinalData(){return &_particles;}
};

#endif