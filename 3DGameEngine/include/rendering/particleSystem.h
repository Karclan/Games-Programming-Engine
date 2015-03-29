#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <memory>

#include "rendering\particleData.h"
#include "rendering\particleEmitters.h"
#include "rendering\particleGenerators.h"
#include "rendering\ParticleUpdaters.h"

class ParticleSystem
{
public:
	ParticleData _particles;

	size_t _particleCount;

	std::vector<SP_ParticleEmitter> _emitters; 
	std::vector<SP_ParticleUpdater> _updaters;

public:
	ParticleSystem(size_t maxCount);
	virtual ~ParticleSystem()
	{
		_updaters.clear();
	}
	 
	void clear();
    virtual void update(float t);
	virtual void reset();
	virtual size_t getParticleCount(){return _particles._particleCount;}

	void addEmitter(SP_ParticleEmitter emitter);
	void addUpdater(SP_ParticleUpdater updater);
	SP_ParticleEmitter getEmitter(int id);
	SP_ParticleUpdater getUpdater(Updaters::type type);

	ParticleData *getFinalData(){return &_particles;}
};

typedef std::shared_ptr<ParticleSystem> SP_ParticleSystem;

#endif