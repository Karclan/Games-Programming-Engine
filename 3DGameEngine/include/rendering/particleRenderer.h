#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#include "rendering\renderer.h"

class ParticleRenderer: public Renderer
{ 
public:
	ParticleRenderer();
	ComponentType::Type getType();

	bool isOnePerObject();

	void linkDependency(SPtr_Component);

	void render(GLfloat* viewMatrix, GLfloat* projectionMatrix);
	
	void play();
	void stop();
	void loop();


private:

};
typedef std::shared_ptr<ParticleRenderer> SPtr_ParticleRend;

#endif