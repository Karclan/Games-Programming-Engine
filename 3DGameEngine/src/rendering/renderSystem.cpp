#include "rendering\renderSystem.h"

RenderSystem::RenderSystem()
{
	setLightDefaults();
}


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

	//if(_unsortedLights.size() != 0) activateLights(); // activate lights if any added

	camera->preRender();
	activateLights();

	//pass 1

	for(unsigned int i = 0; i < _models.size(); ++i)
	{
		// this is where you should check for if it's state is inactive or destroyed
		camera->render(*_models[i]);
	}
	// Unbind vertex array - ensure nothing is left bound to opengl
	glBindVertexArray(0);
	
	//pass 2



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


// Default lighting values
void RenderSystem::setLightDefaults()
{
	setGlobalAmbient	(glm::vec3(0.2f, 0.2f, 0.2f));
	setGlobalDiffuse	(glm::vec3(0.4f, 0.4f, 0.4f));
	setGlobalSpecular	(glm::vec3(1.0f, 1.0f, 1.0f));
	setGlobalDirection	(glm::vec3(0.0f, 0.0f,-1.0f)); // will normalize coz setting through function
}

void RenderSystem::setGlobalAmbient(glm::vec3 ambient)
{
	_globalAmbient = ambient;
}
void RenderSystem::setGlobalDiffuse(glm::vec3 diffuse)
{
	_globalDiffuse = diffuse;
}
void RenderSystem::setGlobalSpecular(glm::vec3 specular)
{
	_globalSpecular = specular;
}
void RenderSystem::setGlobalDirection(glm::vec3 direction)
{
	_globalDirection = glm::normalize(direction);
}

void RenderSystem::clear()
{
	_cameras.clear		();
	_models.clear		(); 
	_animations.clear	();
	_pointLights.clear	();
	_spotLights.clear	();
}

void RenderSystem::activateLights()
{

	for(size_t i=0; i < _unsortedLights.size(); i++)
	{
		switch(_unsortedLights[i]->getLightType())
		{
			case LightType::DIRECTION:
				break;
			case LightType::POINT:
				_pointLights.push_back(_unsortedLights[i]);
				std::cout<<"Added a point light\n";
				break;
			case LightType::SPOT:
				_spotLights.push_back(_unsortedLights[i]);
				std::cout<<"Added a spot light\n";
				break;
			default:
				break;
		}
	}
	_unsortedLights.clear();

	std::map<std::string, Shader*>::const_iterator it;
	it = _loadedShaders->begin();

	std::string index;

	for(it; it != _loadedShaders->end(); ++it)
	{
		it->second->useProgram();

		it->second->setUniform("glDirLight.amb",	  _globalAmbient); // Global Ambient
		it->second->setUniform("glDirLight.diff",   _globalDiffuse); // Global Ambient
		it->second->setUniform("glDirLight.spec",  _globalSpecular); // Global Ambient
		it->second->setUniform("glDirLight.direction", _globalDirection); // Global Ambient

		int pointLights = _pointLights.size();
		int spotLights = _spotLights.size();
		it->second->setUniform("numOfPointLights",pointLights);
		it->second->setUniform("numOfSpotLights",spotLights);
				
		for(int i = 0; i < _pointLights.size(); ++i)
		{
			index = std::to_string(i);
		
			it->second->setUniform( std::string("pointLight["+index+"].position") .c_str(),	 _pointLights[i]->getTransform()->getPosition());
			it->second->setUniform( std::string("pointLight["+index+"].diff")	  .c_str(),	 _pointLights[i]->getDiffuse());
			it->second->setUniform( std::string("pointLight["+index+"].spec")	  .c_str(),	 _pointLights[i]->getSpecular());
			it->second->setUniform( std::string("pointLight["+index+"].constant") .c_str(),	 _pointLights[i]->getAtteunation().x);
			it->second->setUniform( std::string("pointLight["+index+"].linear")	  .c_str(),	 _pointLights[i]->getAtteunation().y);
			it->second->setUniform( std::string("pointLight["+index+"].quadratic").c_str(),	 _pointLights[i]->getAtteunation().z);		
		}
		
		for(int i=0; i<_spotLights.size();++i)
		{
			index = std::to_string(i);
			it->second->setUniform( std::string("spotLight["+index+"].position")  .c_str(),	 _spotLights[i]->getTransform()->getPosition());
			it->second->setUniform( std::string("spotLight["+index+"].spotDir")	  .c_str(),	 _spotLights[i]->getTransform()->getForward());
			it->second->setUniform( std::string("spotLight["+index+"].spotOutCut").c_str(),	 glm::cos(glm::radians(28.f)));
			it->second->setUniform( std::string("spotLight["+index+"].spotInCut") .c_str(),	 glm::cos(glm::radians(14.f)));
			it->second->setUniform( std::string("spotLight["+index+"].diff")	  .c_str(),	 _spotLights[i]->getDiffuse());
			it->second->setUniform( std::string("spotLight["+index+"].spec")	  .c_str(),	 _spotLights[i]->getSpecular());
			it->second->setUniform( std::string("spotLight["+index+"].constant")  .c_str(),	 _spotLights[i]->getAtteunation().x);
			it->second->setUniform( std::string("spotLight["+index+"].linear")	  .c_str(),	 _spotLights[i]->getAtteunation().y);
			it->second->setUniform( std::string("spotLight["+index+"].quadratic") .c_str(),	 _spotLights[i]->getAtteunation().z);
		}
	}
}

void RenderSystem::addLight(SPtr_Light light)
{
	_unsortedLights.push_back(light);
}
