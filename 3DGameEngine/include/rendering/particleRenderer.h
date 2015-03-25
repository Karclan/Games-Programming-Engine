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
	void addEmitter(SP_ParticleEmitter emitter);
	void addGenerator(SP_ParticleGenerator generator);
	void addUpdater(SP_ParticleUpdater updater);

private:

	bool _playFlag;
	bool _generatedFlag;

	GLuint _vao;
	GLuint _particlePositionBuffer;
	GLuint _particleColourBuffer;
	Shader* _shader;

	std::shared_ptr<Particles::Updaters::EulerUpdater> _eulerUpdater;
	Particles::ParticleSystem* _particleSystem;


	SP_ParticleEmitter _emitter;
	std::vector<SP_ParticleUpdater> _updaters;
	std::vector<SP_ParticleGenerator> _generators;

	SPtr_Transform _transform; //The origin of the particleEmitter
};


typedef std::shared_ptr<ParticleRenderer> SPtr_ParticleRend;

#endif