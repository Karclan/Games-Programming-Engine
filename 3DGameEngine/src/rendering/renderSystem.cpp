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

	if(_unsortedLights.size() != 0) activateLights(); // activate lights if any added
	camera->preRender();

	for(unsigned int i = 0; i < _models.size(); ++i)
	{
		// this is where you should check for if it's state is inactive or destroyed
		camera->render(*_models[i]);
	}
	// Unbind vertex array - ensure nothing is left bound to opengl
	glBindVertexArray(0);
	

	FTInterface::renderText("Editor Mode",0,0,1,glm::vec3(0.5f,0.1f,0.8f));

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
	_pointLights.clear();
	_spotLights.clear();
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
		int pointLights = _pointLights.size();
		int spotLights = _spotLights.size();
		it->second->setUniform("numOfPointLights",pointLights);
		std::cout<<"pointlights :" <<_pointLights.size()<<"\n";

		it->second->setUniform("numOfSpotLights",spotLights);
		std::cout<<"spotLights :" <<_spotLights.size()<<"\n";


		for(int i = 0; i < _pointLights.size(); ++i)
		{
			index = std::to_string(i);
			//std::cout<<"index: "<<index<<"\n";
			//std::cout<<std::string("pointLight["+index+"].position").c_str() << " " << _pointLights[i]->getTransform()->getPosition().y<<"\n";
			//std::cout<<"attenuation  :"<<_pointLights[i]->getAtteunation().x<<" "<<_pointLights[i]->getAtteunation().y<<" "<<_pointLights[i]->getAtteunation().z<<"\n";

			it->second->setUniform( std::string("pointLight["+index+"].position") .c_str(),	 _pointLights[i]->getTransform()->getPosition());
			it->second->setUniform( std::string("pointLight["+index+"].amb")	  .c_str(),	 _pointLights[i]->getAmbient());
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
			it->second->setUniform( std::string("spotLight["+index+"].spotDir")	  .c_str(),	 _spotLights[i]->getTransform()->getScale());
			it->second->setUniform( std::string("spotLight["+index+"].spotOutCut").c_str(),	 glm::cos(glm::radians(28.f)));
			it->second->setUniform( std::string("spotLight["+index+"].spotInCut") .c_str(),	 glm::cos(glm::radians(14.f)));
			it->second->setUniform( std::string("spotLight["+index+"].amb")		  .c_str(),	 _spotLights[i]->getAmbient());
			it->second->setUniform( std::string("spotLight["+index+"].diff")	  .c_str(),	 _spotLights[i]->getDiffuse());
			it->second->setUniform( std::string("spotLight["+index+"].spec")	  .c_str(),	 _spotLights[i]->getSpecular());
			it->second->setUniform( std::string("spotLight["+index+"].constant")  .c_str(),	 _spotLights[i]->getAtteunation().x);
			it->second->setUniform( std::string("spotLight["+index+"].linear")	  .c_str(),	 _spotLights[i]->getAtteunation().y);
			it->second->setUniform( std::string("spotLight["+index+"].quadratic") .c_str(),	 _spotLights[i]->getAtteunation().z);
		}


		//glm::vec3 lightPos = glm::vec3(1.f,5.f,0.f);
		//it->second->setUniform("numOfSpotLights",2);
		//it->second->setUniform("spotLight[0].position", lightPos);
		//it->second->setUniform("spotLight[0].spotDir", glm::vec3(-0.0f,-1.f,-0.f));
		//it->second->setUniform("spotLight[0].spotOutCut",glm::cos(glm::radians(28.f)));
		//it->second->setUniform("spotLight[0].spotInCut",glm::cos(glm::radians(14.f)));
		//it->second->setUniform("spotLight[0].constant",1.f);
		//it->second->setUniform("spotLight[0].linear",0.014f);
		//it->second->setUniform("spotLight[0].quadratic",0.0007f);
		//it->second->setUniform("spotLight[0].amb",  0.2f, 0.2f, 0.2f);
		//it->second->setUniform("spotLight[0].diff", 1.f, 0.0f, 0.0f);
		//it->second->setUniform("spotLight[0].spec", 1.f, 0.0f, 0.0f);
		////SpotLight2
		//lightPos = glm::vec3(-1.f,5.f,0.f);
		//it->second->setUniform("spotLight[1].position", lightPos);
		//it->second->setUniform("spotLight[1].spotDir", glm::vec3(-0.0f,-1.f,-0.f));
		//it->second->setUniform("spotLight[1].spotOutCut",glm::cos(glm::radians(28.f)));
		//it->second->setUniform("spotLight[1].spotInCut",glm::cos(glm::radians(14.f)));
		//it->second->setUniform("spotLight[1].constant",1.f);
		//it->second->setUniform("spotLight[1].linear",0.014f);
		//it->second->setUniform("spotLight[1].quadratic",0.0007f);
		//it->second->setUniform("spotLight[1].amb",  0.2f, 0.2f, 0.2f);
		//it->second->setUniform("spotLight[1].diff", 0.0f, 1.0f, 0.0f);
		//it->second->setUniform("spotLight[1].spec", 0.f,  1.f,  0.f);
		////PointLight1
		//lightPos = glm::vec3(0.f,5.f,10.f);
		//it->second->setUniform("numOfPointLights",1);
		//it->second->setUniform("pointLight[0].position", lightPos);
		//it->second->setUniform("pointLight[0].constant",1.f);
		//it->second->setUniform("pointLight[0].linear",0.014f);
		//it->second->setUniform("pointLight[0].quadratic",0.0007f);
		//it->second->setUniform("pointLight[0].amb",  0.2f, 0.2f, 0.2f);
		//it->second->setUniform("pointLight[0].diff", 0.5f, 0.5f, 0.5f);
		//it->second->setUniform("pointLight[0].spec", 0.f,  1.f,  0.f);	



		it->second->setUniform("material.diff",glm::vec3(0.8f,0.8f,0.8f));
		it->second->setUniform("material.spec",glm::vec3(0.5f,0.5f,0.5f));
		it->second->setUniform("material.specEx",128.f);


	}
}

void RenderSystem::addLight(SPtr_Light light)
{
	_unsortedLights.push_back(light);
}
