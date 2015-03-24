#ifndef PARTICLE_DATA_H
#define PARTICLE_DATA_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>

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
	ParticleData(size_t maxCount){generate(maxCount); }
	~ParticleData(){}

	void generate(size_t maxSize);
	void kill(size_t id);
	void wake(size_t id);
	void swapData(size_t a, size_t b);
};


#endif