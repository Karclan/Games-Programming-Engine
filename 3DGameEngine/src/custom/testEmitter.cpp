#include "custom\testEmitter.h"

TestEmitter::TestEmitter()
{

}

void TestEmitter::initialize()
{
	addEventListener(EventType::UPDATE);

	_particleRenderer = std::static_pointer_cast<ParticleRenderer>(getComponent(ComponentType::PARTICLE_REND));

	//_particleRenderer->generate(10000);
}

void TestEmitter::update(float t)
{
	if(Input::getKeyPressed(sf::Keyboard::Space))
	{
		_particleRenderer->stop();
	}
	if(Input::getKeyPressed(sf::Keyboard::W))
	{
		_particleRenderer->play();
	}
}