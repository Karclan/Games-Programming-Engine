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
	void destory();
	void play();
	void stop();

private:

	bool _playFlag;
	bool _generatedFlag;

	GLuint _vao;
	GLuint _particlePositionBuffer;
	GLuint _particleColourBuffer;
	Shader* _shader;

	ParticleSystem* _particleSystem;

	SPtr_Transform _transform; //The origin of the particleEmitter
};


typedef std::shared_ptr<ParticleRenderer> SPtr_ParticleRend;

#endif