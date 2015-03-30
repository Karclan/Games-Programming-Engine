#include "custom\firework.h"


Firework::Firework()
{

}

void Firework::initialize()
{
	_time=0.f;
	_fireworkTime=1.5f;
	_explode=false;
	_exploding=false;

	addEventListener(EventType::UPDATE);

	_particleRenderer = std::static_pointer_cast<ParticleRenderer>(getComponent(ComponentType::PARTICLE_REND));

	_posGenerator.reset(new RoundPosGen());
	_posGenerator->setCenter(glm::vec4(0.0,0.0,0.0,1.0));
	_posGenerator->setXRadius(0.15f);
	_posGenerator->setYRadius(0.15f);
	_colGenerator.reset(new BasicColourGen());
	_colGenerator->setMinStartColour(glm::vec4( 0.5, 0.5, 0.5, 1.0 ));
	_colGenerator->setMaxStartColour(glm::vec4( 1.0, 1.0, 1.0, 1.0 ));
	_colGenerator->setMinEndColour	(glm::vec4( 0.0, 0.0, 0.0, 1.0 ));
	_colGenerator->setMaxEndColour	(glm::vec4( 0.2, 0.2, 0.2, 1.0 ));
	_velGenerator.reset(new BasicVelGen());
	_velGenerator->setMinStartVel	(glm::vec4( -5.45f, -5.45f, -5.45f, 0.0f ));
	_velGenerator->setMaxStartVel	(glm::vec4(  5.45f,  5.45f,  5.45f, 0.0f ));
	_timeGenerator.reset(new BasicTimeGen());
	_timeGenerator->setMinTime(0.5f);
	_timeGenerator->setMaxTime(2.5f);

	_emitter.reset(new ParticleEmitter());
	_emitter->init(100,1.f);
	_emitter->addGenerator(_posGenerator);
	_emitter->addGenerator(_colGenerator);
	_emitter->addGenerator(_velGenerator);
	_emitter->addGenerator(_timeGenerator);
	_particleRenderer->addEmitter(_emitter);

	_timeUpdater.reset(new BasicTimeUpdater());
	_colourUpdater.reset(new BasicColourUpdater());
	_eulerUpdater.reset(new EulerUpdater());

	//_floorUpdater.reset(new FloorUpdater());
	//current suspicion about floor thing is that the actual "floor" it checks against
	//is relative to the particle system's own coordiante system and not world positions

	_eulerUpdater->setGlobalAcc(glm::vec4(0.0,-45.0,0.0,0.0));

	_particleRenderer->clear();//clears updaters
	_particleRenderer->addUpdater(_timeUpdater);
	_particleRenderer->addUpdater(_colourUpdater);
	_particleRenderer->addUpdater(_eulerUpdater);
	//_particleRenderer->addUpdater(_floorUpdater);

	_particleRenderer->generate(10000);
	_particleRenderer->setEmitFlag(false);
	_particleRenderer->resetParticles();
}

void Firework::update(float t)
{
	
	if(Input::getKeyPressed(sf::Keyboard::Num1))
	{
		_particleRenderer->setParticlePointSize(10.f);
		_explode=true;
	}

	if(_explode)
	{
		_exploding=true;
	}

	//do the explode
	if(_exploding)
	{
		_time+=t;
		if(_time>=_fireworkTime)
		{
			_time=0.f;
			_exploding=false;
			_explode=false;
			_particleRenderer->setEmitFlag(false);
		}
		else
		{
			_particleRenderer->setEmitFlag(true);
		}
	}
	
}

