#include "rendering\particleRenderer.h"

ParticleRenderer::ParticleRenderer()
{
	setDepFlag(ComponentType::TRANSFORM);
	_playFlag=true;

	_shader = Assets::getShader("particle");
}

ComponentType::Type ParticleRenderer::getType()
{
	return ComponentType::PARTICLE_REND;
}

bool ParticleRenderer::isOnePerObject()
{
	return true;
}

void ParticleRenderer::linkDependency(SPtr_Component c)
{
	switch(c->getType())
	{
	case ComponentType::TRANSFORM:
		_transform = std::static_pointer_cast<Transform>(c);
		break;
	}
}

void ParticleRenderer::generate(size_t particlePool)
{
	_particleSystem = new ParticleSystem(particlePool);
	ab.init(particlePool);

	_particleSystem->addEmitter(SP_CircleEmitter(&ab));

	auto timeUpdater = std::make_shared<BasicTimeUpdater>();
	_particleSystem->addUpdater(timeUpdater);

	auto colourUpdater = std::make_shared<BasicColourUpdater>();
	_particleSystem->addUpdater(colourUpdater);

	_eulerUpdater = std::make_shared<EulerUpdater>();
	{
		_eulerUpdater->_globalAcceleration=glm::vec4(0.0,15.0,0.0,0.0);
	}
	_particleSystem->addUpdater(_eulerUpdater);


	const size_t count = _particleSystem->getParticleCount();

	glGenVertexArrays(1,&_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1,&_particlePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _particlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*count,nullptr,GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,(4)*sizeof(float),(void*)((0)*sizeof(float)));


	glGenBuffers(1,&_particleColourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,_particleColourBuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float)*4*count,nullptr,GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,(4)*sizeof(float),(void*)((0)*sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
}

void ParticleRenderer::render(GLfloat* viewMatrix, GLfloat *projectionMatrix)
{
	_shader->useProgram();
	ab.posGenerator->_center=glm::vec4(_transform->getPosition(),1.0);

	glEnable(GL_PROGRAM_POINT_SIZE);

	_shader->setUniform("u_ModelMatrix",_transform->getMatrix());
	_shader->setUniform("u_ViewMatrix",viewMatrix);
	_shader->setUniform("u_ProjectionView",projectionMatrix);

	glBindVertexArray(_vao);

	const size_t count = _particleSystem->getParticleCount();
	if(count > 0)
	{
		glDrawArrays(GL_POINTS , 0 , count);
	}

	glBindVertexArray(0);

	glDisable(GL_PROGRAM_POINT_SIZE);
}

void ParticleRenderer::animate(float t)
{
	if(_playFlag)
	{
		_particleSystem->update(t);

		const size_t count = _particleSystem->getParticleCount();
		if(count > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _particlePositionBuffer);
			float *ptr = (float *)(_particleSystem->getFinalData()->_particlePositions.get());
			glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float)*4, ptr);

			glBindBuffer(GL_ARRAY_BUFFER, _particleColourBuffer);
			ptr = (float *)(_particleSystem->getFinalData()->_particleColours.get());
			glBufferSubData(GL_ARRAY_BUFFER, 0, count*sizeof(float)*4,ptr);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
}

void ParticleRenderer::play()
{
	_playFlag=true;
}

void ParticleRenderer::stop()
{
	_playFlag=false;
}
