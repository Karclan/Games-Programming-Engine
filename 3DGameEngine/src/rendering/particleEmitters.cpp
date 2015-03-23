#include "rendering\particleEmitters.h"

TestCircleEmitter::TestCircleEmitter(size_t particlePool)
{
		_emitRate=(float)particlePool*0.45f;

		SP_RoundPosGen posGenerator = std::make_shared<RoundPosGen>();
		posGenerator->_center = glm::vec4(0.0,0.0,0.0,1.0);
		posGenerator->_radiusX=0.15f;
		posGenerator->_radiusY=0.15f;
		addGenerator(posGenerator);
	
		SP_BasicColourGen colGenerator = std::make_shared<BasicColourGen>();
		colGenerator->_minStartColour	= glm::vec4( 0.0, 0.5, 0.0, 1.0 );
		colGenerator->_maxStartColour	= glm::vec4( 0.0, 1.0, 0.0, 1.0 );
		colGenerator->_minEndColour		= glm::vec4( 0.5, 0.0, 0.0, 0.0 );
		colGenerator->_maxEndColour	    = glm::vec4( 1.0, 0.0, 0.0, 0.0 );
		addGenerator(colGenerator);

		SP_BasicVelGen velGenerator = std::make_shared<BasicVelGen>();
		velGenerator->_minStartVel = glm::vec4( 0.0f, 0.0f, 0.15f, 0.0f );
		velGenerator->_maxStartVel = glm::vec4( 1.0f, 0.0f, 0.45f, 0.0f );
		addGenerator(velGenerator);

		SP_BasicTimeGen timeGenerator = std::make_shared<BasicTimeGen>();
		timeGenerator->_minTime = 1.0;
		timeGenerator->_maxTime = 3.5;
		addGenerator(timeGenerator);
}