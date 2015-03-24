#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "rendering\particleData.h"
#include "rendering\particleEmitters.h"
#include "rendering\particleGenerators.h"
#include "rendering\ParticleUpdaters.h"

class ParticleSystem
{
protected:
	ParticleData _particles;

	size_t _particleCount;

	std::vector<SP_ParticleEmitter> _emitters; 
	std::vector<SP_ParticleUpdater> _updaters;

public:
	ParticleSystem(size_t maxCount);
	virtual ~ParticleSystem(){}

	virtual void update(float t);
	virtual void reset();
	virtual size_t getParticleCount()const{return _particles._particleCount;}

	void addEmitter(SP_ParticleEmitter emitter){_emitters.push_back(emitter);}
	void addUpdater(SP_ParticleUpdater updater){_updaters.push_back(updater);}
	SP_ParticleEmitter getEmitter(Emitters::type type);
	SP_ParticleUpdater getUpdater(Updaters::type type);

	ParticleData *getFinalData(){return &_particles;}

};

#endif