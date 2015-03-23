#ifndef PARTICLEUPDATERS_H
#define PARTICLEUPDATERS_H

#include <glm/glm.hpp>

#include "rendering/particle.h"




class EulerUpdater : public ParticleUpdater
{
public:
	glm::vec4 _globalAcceleration;

	EulerUpdater(){_globalAcceleration=glm::vec4(0.f);}

	virtual void update(float dt, ParticleData *p) override;
};

class AttractorUpdater : public ParticleUpdater
{
public:
	std::vector<glm::vec4> _attractors;

	virtual void update(float dt, ParticleData *p) override;

	size_t collectionSize() const {return _attractors.size();}
	void addAttractor(const glm::vec4 &attractor){_attractors.push_back(attractor);}
	glm::vec4 &getAttractor(size_t id){return _attractors[id];}
};

class BasicColourUpdater : public ParticleUpdater
{
public:
	virtual void update(float dt, ParticleData *p) override;
};

class PositionColourUpdater : ParticleUpdater
{
public:
	glm::vec4 _minPosition;
	glm::vec4 _maxPosition;

	virtual void update(float dt, ParticleData *p) override;
};

class VelocityColourUpdater : public ParticleUpdater
{
	glm::vec4 _minVelocity;
	glm::vec4 _maxVelocity;

	virtual void update(float dt, ParticleData *p) override;
};

class BasicTimeUpdater : public ParticleUpdater
{
public:
	virtual void update(float dt, ParticleData *p) override;
};


#endif