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

	SP_ParticleGenerator getGenerator(Generators::type type);
	SP_ParticleUpdater	 getUpdater(Updaters::type type);
	SP_CircleEmitter circleEmitter;

	SP_EulerUpdater _eulerUpdater;
	SP_FloorUpdater _floorUpdater;

private:

	bool _playFlag;
	bool _generatedFlag;

	GLuint _vao;
	GLuint _particlePositionBuffer;
	GLuint _particleColourBuffer;
	Shader* _shader;

	int				_particlePool;
	SP_ParticleSystem _particleSystem;

	std::vector<SP_ParticleUpdater>	  _updaters;

	SPtr_Transform _transform; //The origin of the particleEmitter
};


typedef std::shared_ptr<ParticleRenderer> SPtr_ParticleRend;

#endif