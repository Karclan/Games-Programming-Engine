#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#include <vector>
#include <glm\glm.hpp>

#include "rendering\particleData.h"
#include "rendering\particleEmitters.h"
#include "rendering\particleGenerators.h"
#include "rendering\particleUpdaters.h"
#include "rendering\particleSystem.h"

#include "rendering\renderer.h"
#include "rendering\shader.h"
#include "core\transform.h"
#include "core\assets.h"

namespace
{
	enum EMITTERS{CIRCLE_EMITTER};
}
class ParticleRenderer: public Renderer
{ 
public:
	ParticleRenderer();
	~ParticleRenderer(){}
	ComponentType::Type getType();

	bool isOnePerObject();

	void linkDependency(SPtr_Component);
	void generate(size_t particlePool);
	void render(GLfloat* viewMatrix, GLfloat* projectionMatrix);
	void animate(float t);

	void play();
	void stop();

	void setPoolSize(size_t poolSize);
	int getPoolSize(){return _particlePool;}
	//void addEmitter(SP_ParticleEmitter emitter);
	//void addGenerator(SP_ParticleGenerator generator);
	//void addUpdater(SP_ParticleUpdater updater);
	
	void activateEmitter(Emitters::type type);
	void activateGenerator(Generators::type type);
	void activateUpdater(Updaters::type type);

	ParticleEmitter&  getEmitter(){return _emitter;}
	SP_ParticleGenerator getGenerator(Generators::type type);
	SP_ParticleUpdater	 getUpdater(Updaters::type type);

	ParticleEmitter _emitter;
	//Generators
	BoxPosGen _boxPosGen;
	RoundPosGen _roundPosGen;
	BasicColourGen _basicColourGen;
	BasicVelGen _basicVelGen;
	SphereVelGen _sphereVelGen;
	BasicTimeGen _basicTimeGen;
	//Updaters
	EulerUpdater _eulerUpdater;
	AttractorUpdater _attractorUpdater;
	BasicColourUpdater _basicColourUpdater;
	PositionColourUpdater _positionColourUpdater;
	VelocityColourUpdater _velocityColourUpdater;
	BasicTimeUpdater _basicTimeUpdate;

private:

	bool _playFlag;
	bool _generatedFlag;

	GLuint _vao;
	GLuint _particlePositionBuffer;
	GLuint _particleColourBuffer;
	Shader* _shader;

	int				_particlePool;
	ParticleSystem* _particleSystem;

	std::vector<SP_ParticleUpdater>	  _updaters;

	SPtr_Transform _transform; //The origin of the particleEmitter
};


typedef std::shared_ptr<ParticleRenderer> SPtr_ParticleRend;

#endif