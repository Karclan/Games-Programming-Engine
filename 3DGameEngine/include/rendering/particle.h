#ifndef PARTICLE_DATA_H
#define PARTICLE_DATA_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Particles
{
	class ParticleData
	{
	public:
		std::unique_ptr <glm::vec4[]> _particlePositions;
		std::unique_ptr <glm::vec4[]> _particleVelocities;
		std::unique_ptr <glm::vec4[]> _particleAccelerations;
		std::unique_ptr <glm::vec4[]> _particleStartColours;
		std::unique_ptr <glm::vec4[]> _particleEndColours;
		std::unique_ptr <glm::vec4[]> _particleColours;
		std::unique_ptr <glm::vec4[]> _particleLifeSpans;
		std::unique_ptr <bool[]> _particleAliveFlags;

		size_t _particleCount;
		size_t _aliveParticleCount;

	public:

		ParticleData(){};
		explicit ParticleData(size_t maxCount){generate(maxCount); }
		~ParticleData(){}

		ParticleData(const ParticleData&); //= delete;
		ParticleData &operator =(const ParticleData &); //= delete;

		void generate(size_t maxSize);
		void kill(size_t id);
		void wake(size_t id);
		void swapData(size_t a, size_t b);
	};

	class ParticleGenerator
	{
	public:
		ParticleGenerator(){}
		virtual ~ParticleGenerator(){}
		virtual void generate(float dt, ParticleData *p, size_t startId, size_t endId)=0;
	};

    typedef std::shared_ptr<ParticleGenerator> SP_ParticleGenerator;

	class ParticleEmitter
	{
	protected:
		std::vector<SP_ParticleGenerator> _generators;
	public:
		float _emitRate;
		ParticleEmitter(){_emitRate=0.0f;}
		virtual ~ParticleEmitter(){}

		virtual void emit(float t, ParticleData *p);

		void addGenerator(SP_ParticleGenerator gen){ _generators.push_back(gen);}
	};

	typedef std::shared_ptr<ParticleEmitter> SP_ParticleEmitter;

	class ParticleUpdater
	{
	public:
		ParticleUpdater(){}
		virtual ~ParticleUpdater(){}

		virtual void update(float t, ParticleData *p)=0;
	};

	typedef std::shared_ptr<ParticleUpdater> SP_ParticleUpdater;

	class ParticleSystem
	{
	protected:
		ParticleData _particles;
		ParticleData _aliveParticles;

		size_t _particleCount;

		std::vector<SP_ParticleEmitter> _emitters; 
		std::vector<SP_ParticleUpdater> _updaters;

	public:
		//ParticleSystem();
		explicit ParticleSystem(size_t maxCount);
		virtual ~ParticleSystem(){}

		ParticleSystem(const ParticleSystem &) ;//= delete;
		ParticleSystem &operator=(const ParticleSystem &);//=delete;

		virtual void update(float t);
		virtual void reset();

		virtual size_t getParticleCount()const{return _particles._particleCount;}
		virtual size_t getAliveParticleCount() const{return _aliveParticles._particleCount;}

		void addEmitter(SP_ParticleEmitter emitter){_emitters.push_back(emitter);}
		void addUpdater(SP_ParticleUpdater updater){_updaters.push_back(updater);}

		ParticleData *getFinalData(){return &_particles;}

		//static size_t computeMemoryUsage(const ParticleSystem &p);
	};
}
#endif