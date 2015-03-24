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

	std::vector<ParticleEmitter*> _emitters; 
	std::vector<ParticleUpdater*> _updaters;

public:
	ParticleSystem(size_t maxCount);
	virtual ~ParticleSystem()
	{
		_emitters.clear();
		_updaters.clear();
	}

	void clear();
    virtual void update(float t);
	virtual void reset();
	virtual size_t getParticleCount(){return _particles._particleCount;}

	void addEmitter(ParticleEmitter* emitter){_emitters.push_back(emitter);}
	void addUpdater(ParticleUpdater* updater){_updaters.push_back(updater);}
	ParticleEmitter* getEmitter(Emitters::type type);
	ParticleUpdater* getUpdater(Updaters::type type);

	ParticleData *getFinalData(){return &_particles;}

};

#endif