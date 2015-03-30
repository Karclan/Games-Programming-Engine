#include "custom\testEmitter.h"

TestEmitter::TestEmitter()
{

}

void TestEmitter::initialize()
{
	addEventListener(EventType::UPDATE);

	_particleRenderer = std::static_pointer_cast<ParticleRenderer>(getComponent(ComponentType::PARTICLE_REND));

	_posGenerator.reset(new RoundPosGen());
	_posGenerator->setCenter(glm::vec4(0.0,0.0,0.0,1.0));
	_posGenerator->setXRadius(0.15f);
	_posGenerator->setYRadius(0.15f);
	_colGenerator.reset(new BasicColourGen());
	_colGenerator->setMinStartColour(glm::vec4( 0.0, 0.5, 0.0, 1.0 ));
	_colGenerator->setMaxStartColour(glm::vec4( 0.0, 1.0, 0.0, 1.0 ));
	_colGenerator->setMinEndColour(glm::vec4( 0.5, 0.0, 0.0, 0.0 ));
	_colGenerator->setMaxEndColour(glm::vec4( 1.0, 0.0, 0.0, 0.0 ));
	_velGenerator.reset(new BasicVelGen());
	_velGenerator->setMinStartVel(glm::vec4( 0.0f, 0.15f, 0.15f, 0.0f ));
	_velGenerator->setMaxStartVel(glm::vec4( 1.0f, 0.15f, 0.45f, 0.0f ));
	_timeGenerator.reset(new BasicTimeGen());
	_timeGenerator->setMinTime(1.0f);
	_timeGenerator->setMaxTime(3.5f);

	_emitter.reset(new ParticleEmitter());
	_emitter->init(10000,0.45f);
	_emitter->addGenerator(_posGenerator);
	_emitter->addGenerator(_colGenerator);
	_emitter->addGenerator(_velGenerator);
	_emitter->addGenerator(_timeGenerator);
	_particleRenderer->addEmitter(_emitter);

	_timeUpdater.reset(new BasicTimeUpdater());
	_colourUpdater.reset(new BasicColourUpdater());
	_eulerUpdater.reset(new EulerUpdater());

	_eulerUpdater->setGlobalAcc(glm::vec4(0.0,-15.0,0.0,0.0));

	_particleRenderer->clear();//clears updaters
	_particleRenderer->addUpdater(_timeUpdater);
	_particleRenderer->addUpdater(_colourUpdater);
	_particleRenderer->addUpdater(_eulerUpdater);

	_particleRenderer->generate(10000);
	_particleRenderer->resetParticles();
}

void TestEmitter::update(float t)
{
	if(Input::getKeyPressed(sf::Keyboard::A))
	{
		_particleRenderer->setAnimFlag(true);
	}
	if(Input::getKeyPressed(sf::Keyboard::B))
	{
		_particleRenderer->setAnimFlag(false);
	}
}