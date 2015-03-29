#include "custom\firework.h"


Firework::Firework()
{

}

void Firework::initialize()
{
	_time=0.f;
	_fireworkTime=3.0f;
	_explode=false;
	_exploding=false;

	addEventListener(EventType::UPDATE);

	_particleRenderer = std::static_pointer_cast<ParticleRenderer>(getComponent(ComponentType::PARTICLE_REND));

	_posGenerator.reset(new RoundPosGen());
	_posGenerator->setCenter(glm::vec4(0.0,0.0,0.0,1.0));
	_posGenerator->setXRadius(0.15f);
	_posGenerator->setYRadius(0.15f);
	_colGenerator.reset(new BasicColourGen());
	_colGenerator->setMinStartColour(glm::vec4( 0.0, 0.5, 0.0, 1.0 ));
	_colGenerator->setMaxStartColour(glm::vec4( 0.0, 1.0, 0.0, 1.0 ));
	_colGenerator->setMinEndColour	(glm::vec4( 0.5, 0.0, 0.0, 0.0 ));
	_colGenerator->setMaxEndColour	(glm::vec4( 1.0, 0.0, 0.0, 0.0 ));
	_velGenerator.reset(new BasicVelGen());
	_velGenerator->setMinStartVel	(glm::vec4( 0.0f, 0.0f, 0.15f, 0.0f ));
	_velGenerator->setMaxStartVel	(glm::vec4( 1.0f, 0.0f, 0.45f, 0.0f ));
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

	//_floorUpdater.reset(new FloorUpdater());
	//current suspicion about floor thing is that the actual "floor" it checks against
	//is relative to the particle system's own coordiante system and not world positions

	_eulerUpdater->setGlobalAcc(glm::vec4(0.0,-15.0,0.0,0.0));

	_particleRenderer->clear();//clears updaters
	_particleRenderer->addUpdater(_timeUpdater);
	_particleRenderer->addUpdater(_colourUpdater);
	_particleRenderer->addUpdater(_eulerUpdater);
	//_particleRenderer->addUpdater(_floorUpdater);

	_particleRenderer->generate(10000);
	_particleRenderer->setDispFlag(false);
	_particleRenderer->setAnimFlag(false);
}

void Firework::update(float t)
{
	if(Input::getKeyPressed(sf::Keyboard::Num1))
	{
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
			_particleRenderer->setDispFlag(false);
			_particleRenderer->setAnimFlag(false);
		}
		else
		{
			_particleRenderer->setDispFlag(true);
			_particleRenderer->setAnimFlag(true);
		}
	}
	
}

