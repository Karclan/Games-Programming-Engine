#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#include <vector>
#include <glm\glm.hpp>

#include "rendering\particleEmitters.h"
#include "rendering\particleGenerators.h"
#include "rendering\particleUpdaters.h"
#include "rendering\particle.h"

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
	void destory();

	void play();
	void stop();
	void loop(bool l);


private:
	bool _loopFlag;
	bool _playFlag;
	GLuint _vao;
	GLuint _particlePositionBuffer;
	GLuint _particleColourBuffer;
	Shader* _shader;

	std::shared_ptr<EulerUpdater> _eulerUpdater;
	ParticleSystem* _particleSystem;

	TestCircleEmitter* ab;

	SPtr_Transform _transform; //The origin of the particleEmitter
};




typedef std::shared_ptr<ParticleRenderer> SPtr_ParticleRend;

#endif