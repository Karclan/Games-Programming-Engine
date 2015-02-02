#include "rendering\renderSystem.h"

void RenderSystem::render()
{
	if(_cameras.size() <= 0) return;
	render(_cameras[0].get()); // call version of render that takes camera pointer as an arg
}


void RenderSystem::render(Camera* camera)
{
	// Here are some things to think about in the future, for optimization.
	// Performance overheads are binding and draw calls. So if you have multiple
	// instances of the same game object you ideally want to draw them together, 
	// with one binding. Ditto for materials.
	// e.g.
	// - bind material
	// - bind 1st mesh that uses it
	// - draw all instances of mesh
	// - bind 2nd mesh that uses it etc etc

	// Draw calls would be harder to manage, needs more research
	
	
	camera->preRender();

	for(unsigned int i = 0; i < _models.size(); ++i)
	{
		// this is where you should check for if it's state is inactive or destroyed
		camera->render(*_models[i]);
	}

	// Unbind vertex array - ensure nothing is left bound to opengl
	glBindVertexArray(0);
}


void RenderSystem::animate(float t)
{
	for(unsigned int i = 0; i < _animations.size(); ++i)
	{
		_animations[i]->animate(t);
	}
}


void RenderSystem::addCamera(std::shared_ptr<Camera> camera )
{
	if(!camera)return;
	_cameras.push_back(camera);
}


void RenderSystem::addRenderObject(SPtr_Renderer renderer)
{
	if(!renderer)return;
	_models.push_back(renderer);
}

void RenderSystem::addAnimatedObject(SPtr_Renderer renderer)
{
	if(!renderer)return;
	_models.push_back(renderer);
	_animations.push_back(renderer);
}



void RenderSystem::clear()
{
	_cameras.clear();
	_models.clear(); 
	_animations.clear();
}

void RenderSystem::setLight(glm::vec3 direction, glm::vec3 intensity)
{
	direction = glm::normalize(direction);

	std::map<std::string, Shader*>::const_iterator it;
	it = _loadedShaders->begin();

	for(it; it != _loadedShaders->end(); ++it)
	{
		it->second->setDirectionalLight(&direction.x, &intensity.x);
	}
}