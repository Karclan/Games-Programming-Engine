#ifndef PARTICLEUPDATERS_H
#define PARTICLEUPDATERS_H

#include <glm/glm.hpp>

#include "rendering\particleData.h"


namespace Updaters
{
	enum type{EULERUPDATER, ATTRACTORUPDATER, BASICCOLOURUPDATER, POSITIONCOLOURUPDATER, VELOCITYCOLOURUPDATER, BASICTIMEUPDATER, UPDATERS};
}

class ParticleUpdater
{
public:
	ParticleUpdater(){}
	virtual ~ParticleUpdater(){}

	virtual Updaters::type getType()=0;

	virtual void update(float t, ParticleData *p)=0;
};

class EulerUpdater : public ParticleUpdater
{
public:
	glm::vec4 _globalAcceleration;

	EulerUpdater(){_globalAcceleration=glm::vec4(0.f);}

	EulerUpdater(glm::vec4 globalAcceleration){_globalAcceleration=globalAcceleration;}

	Updaters::type getType(){return Updaters::EULERUPDATER;}

	virtual void update(float dt, ParticleData *p);
};

class AttractorUpdater : public ParticleUpdater
{
public:
	std::vector<glm::vec4> _attractors;
	
	Updaters::type getType(){return Updaters::ATTRACTORUPDATER;}

	virtual void update(float dt, ParticleData *p);

	size_t collectionSize() const {return _attractors.size();}
	void addAttractor(const glm::vec4 &attractor){_attractors.push_back(attractor);}
	glm::vec4 &getAttractor(size_t id){return _attractors[id];}
};

class BasicColourUpdater : public ParticleUpdater
{
public:
	Updaters::type getType(){return Updaters::BASICCOLOURUPDATER;}

	virtual void update(float dt, ParticleData *p);
};

class PositionColourUpdater : ParticleUpdater
{
public:
	glm::vec4 _minPosition;
	glm::vec4 _maxPosition;

	Updaters::type getType(){return Updaters::POSITIONCOLOURUPDATER;}

	virtual void update(float dt, ParticleData *p);
};

class VelocityColourUpdater : public ParticleUpdater
{
	glm::vec4 _minVelocity;
	glm::vec4 _maxVelocity;

	Updaters::type getType(){return Updaters::VELOCITYCOLOURUPDATER;}

	virtual void update(float dt, ParticleData *p);
};

class BasicTimeUpdater : public ParticleUpdater
{
public:
	Updaters::type getType(){return Updaters::BASICTIMEUPDATER;}

	virtual void update(float dt, ParticleData *p);
};

#endif