#include "rendering\particleRenderer.h"

ParticleRenderer::ParticleRenderer()
{
	setDepFlag(ComponentType::TRANSFORM);
	_playFlag=false;
	_loopFlag=false;

	std::cout<<"Made Particle Renderer!\n";
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
	_particleSystem = new Particles::ParticleSystem(particlePool);

	auto particleEmitter = std::make_shared<Particles::ParticleEmitter>();
	{
		particleEmitter->_emitRate=(float)particlePool*0.45f;

		auto posGenerator = std::make_shared<Particles::Generators::RoundPosGen>();
		posGenerator->_center = glm::vec4(0.f);
		posGenerator->_radiusX=0.15f;
		posGenerator->_radiusY=0.15f;
		particleEmitter->addGenerator(posGenerator);

		auto colGenerator = std::make_shared<Particles::Generators::BasicColourGen>();
		colGenerator->_minStartColour	= glm::vec4( 0.7, 0.0, 0.7, 1.0 );
		colGenerator->_maxStartColour	= glm::vec4( 1.0, 1.0, 1.0, 1.0 );
		colGenerator->_minEndColour		= glm::vec4( 0.5, 0.0, 0.6, 0.0 );
		colGenerator->_maxStartColour	= glm::vec4( 0.7, 0.5, 1.0, 0.0 );
		particleEmitter->addGenerator(colGenerator);

		auto velGenerator = std::make_shared<Particles::Generators::BasicVelGen>();
		velGenerator->_minStartVel = glm::vec4( 0.0f, 0.0f, 0.15f, 0.0f );
		velGenerator->_maxStartVel = glm::vec4( 0.0f, 0.0f, 0.45f, 0.0f );
		particleEmitter->addGenerator(velGenerator);

		auto timeGenerator = std::make_shared<Particles::Generators::BasicTimeGen>();
		timeGenerator->_minTime = 1.0;
		timeGenerator->_maxTime = 3.5;
		particleEmitter->addGenerator(timeGenerator);
	}
	_particleSystem->addEmitter(particleEmitter);

	auto timeUpdater = std::make_shared<Particles::Updaters::BasicTimeUpdater>();
	_particleSystem->addUpdater(timeUpdater);

	auto colourUpdater = std::make_shared<Particles::Updaters::BasicColourUpdater>();
	_particleSystem->addUpdater(colourUpdater);

	_eulerUpdater = std::make_shared<Particles::Updaters::EulerUpdater>();
	{
		_eulerUpdater->_globalAcceleration=glm::vec4(0.0,-15.0,0.0,0.0);
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

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,0);
}
void ParticleRenderer::render(GLfloat* viewMatrix, GLfloat *projectionMatrix)
{
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	_shader->useProgram();

	_shader->setUniform("u_ModelView",viewMatrix);
	_shader->setUniform("u_ProjectionView",projectionMatrix);

	glBindVertexArray(_vao);

	const size_t count = _particleSystem->getAliveParticleCount();
	if(count > 0)
	{
		glDrawArrays(GL_POINTS , 0 , count);
	}
	glBindVertexArray(0);

	glDisable(GL_BLEND);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void ParticleRenderer::animate(float t)
{
	_particleSystem->update(t);

	const size_t count = _particleSystem->getAliveParticleCount();
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

void ParticleRenderer::play()
{
	_playFlag=true;
}

void ParticleRenderer::stop()
{
	_playFlag=false;
}

void ParticleRenderer::loop(bool l)
{
	_loopFlag=l;
}