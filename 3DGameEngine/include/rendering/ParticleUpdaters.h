#ifndef PARTICLEUPDATERS_H
#define PARTICLEUPDATERS_H

#include <glm/glm.hpp>

#include "rendering\particleData.h"


namespace Updaters
{
	enum type{EULERUPDATER, FLOORUPDATER, ATTRACTORUPDATER, BASICCOLOURUPDATER, POSITIONCOLOURUPDATER, VELOCITYCOLOURUPDATER, BASICTIMEUPDATER, UPDATERS};
}

class ParticleUpdater
{
private: 
	bool _active;
public:
	ParticleUpdater(){_active=false;}
	virtual ~ParticleUpdater(){}

	virtual Updaters::type getType()=0;

	virtual void update(float t, ParticleData *p)=0;
	virtual bool isActive(){return _active;}
	virtual void setActiveFlag(bool f){_active = f;}
};

typedef std::shared_ptr<ParticleUpdater> SP_ParticleUpdater;

class EulerUpdater : public ParticleUpdater
{
private:
	glm::vec4 _globalAcceleration;
public:
	
	EulerUpdater(){_globalAcceleration=glm::vec4(0.f);}

	EulerUpdater(glm::vec4 globalAcceleration){_globalAcceleration=globalAcceleration;}

	Updaters::type getType(){return Updaters::EULERUPDATER;}

	virtual void update(float dt, ParticleData *p);

	glm::vec4 getGlobalAcc(){return _globalAcceleration;}
	void setGlobalAcc(glm::vec4 acc){_globalAcceleration=acc;}

};

typedef std::shared_ptr<EulerUpdater> SP_EulerUpdater;

class FloorUpdater : public ParticleUpdater
{
private:
	glm::vec3 _wall;
	float _bounceFactor;
public:
	FloorUpdater(){_wall=glm::vec3(0.f);_bounceFactor=0.5f;}
	Updaters::type getType(){return Updaters::FLOORUPDATER;}
	virtual void update(float dt, ParticleData *p);
};

typedef std::shared_ptr<FloorUpdater> SP_FloorUpdater;

class AttractorUpdater : public ParticleUpdater
{
private:
	glm::vec4 _attractors[4];
	bool	  _attActFlags[4];
public:
	AttractorUpdater(){
		for(int i = 0; i < 4; ++i)
		{
			_attractors [i]=glm::vec4(0.f);
			_attActFlags[i]=false;
		}
	}

	Updaters::type getType(){return Updaters::ATTRACTORUPDATER;}
	virtual void update(float dt, ParticleData *p);
	size_t collectionSize() {return 4;}
	void addAttractor(int id, bool f, glm::vec4 &attractor){
		if(id<3)return;
		_attractors [id]=attractor;
		_attActFlags[id]=f;
	}
	void toggleAttractor(int id, bool flag){
		if(id<3)return;
		_attActFlags[id]=flag;
	}
	bool getAttractorState(int id)
	{
		if(id<3) return false;
		return _attActFlags[id];
	}
	glm::vec4 getAttractor(size_t id){
		if(id<3)return glm::vec4(0.f); 
		return _attractors[id];
	}
};

typedef std::shared_ptr<AttractorUpdater> SP_AttractorUpdater;

class BasicColourUpdater : public ParticleUpdater
{
public:
	Updaters::type getType(){return Updaters::BASICCOLOURUPDATER;}
	virtual void update(float dt, ParticleData *p);
};

typedef std::shared_ptr<BasicColourUpdater> SP_BasicColourUpdater;

class PositionColourUpdater : public ParticleUpdater
{
private:
	glm::vec4 _minPosition;
	glm::vec4 _maxPosition;
public:
	PositionColourUpdater()
	{
		_minPosition=glm::vec4(0.f);
		_maxPosition=glm::vec4(0.f);
	}

	Updaters::type getType(){return Updaters::POSITIONCOLOURUPDATER;}

	glm::vec4 getMinPos()
	{
		return _minPosition;
	}
	glm::vec4 getMaxPos()
	{
		return _maxPosition;
	}
	void setMinPos(glm::vec4 pos){_minPosition=pos;}
	void setMaxPos(glm::vec4 pos){_maxPosition=pos;}
	virtual void update(float dt, ParticleData *p);
};

typedef std::shared_ptr<PositionColourUpdater> SP_PositionColourUpdater;

class VelocityColourUpdater : public ParticleUpdater
{
private:
	glm::vec4 _minVelocity;
	glm::vec4 _maxVelocity;
public:
	Updaters::type getType(){return Updaters::VELOCITYCOLOURUPDATER;}

	virtual void update(float dt, ParticleData *p);

	glm::vec4 getMinVel(){return _minVelocity;}
	glm::vec4 getMaxVel(){return _maxVelocity;}
	void setMinVel(glm::vec4 vel){_minVelocity = vel;}
	void setMaxVel(glm::vec4 vel){_maxVelocity = vel;}
};

typedef std::shared_ptr<VelocityColourUpdater> SP_VelocityColourUpdater;

class BasicTimeUpdater : public ParticleUpdater
{
public:
	Updaters::type getType(){return Updaters::BASICTIMEUPDATER;}

	virtual void update(float dt, ParticleData *p);
};

typedef std::shared_ptr<BasicTimeUpdater> SP_BasicTimeUpdater;

#endif