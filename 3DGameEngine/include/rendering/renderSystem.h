#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <vector>
#include <map>

#include "rendering\camera.h"
#include "rendering\renderer.h"
#include "rendering\shader.h"
#include "rendering\light.h"
#include "rendering\animator.h"
#include "rendering\particleRenderer.h"
#include "rendering\FTInterface.h"
#include "rendering\GUI.h"


/*! \brief Render Sub-System

	System for managing and rendering all rendable objects

*/
namespace Cubemap
{
	enum sides{POSX,POSY,POSZ,NEGX,NEGY,NEGZ,SIDES};
};

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem(){};

	void render(); //!< Render the camera through the currently selected camera
	void render(Camera* camera); //!< Render the scene through a camera passed as an arg
	void animate(float t); //!< Call animate on any animation components
	
	void addCamera(SPtr_Camera camera); //!< Add a new camera to the system
	void addLight(SPtr_Light light);
	void addRenderObject(SPtr_Renderer renderer); //!< Add a static (unanimated) model

	void addAnimator(SPtr_Animator anim); 
	void addParticleSystem(SPtr_ParticleRend particle);


	void createSkybox();
	void addGUI(SPtr_GUI gui);

	void removeCamera(SPtr_Camera camera); //!< Remove Camera from system
	void removeLight(SPtr_Light light);
	void removeRenderObject(SPtr_Renderer renderer); //!< Remove Renderer
	void removeAnimator(SPtr_Animator anim);  //!< Remove a given animator
	void removeGUI(SPtr_GUI gui);

	void setLightDefaults(); //!< Set default values for the global light params
	void setSkyboxSide(std::string, Cubemap::sides);//!< Sets the side of the skybox
	void renderSkybox();//!< Function to render skybox, no reason to have it anywhere else

	void setGlobalAmbient	(glm::vec3 ambient  );
	void setGlobalDiffuse	(glm::vec3 diffuse  );
	void setGlobalSpecular  (glm::vec3 specular );
	void setGlobalDirection (glm::vec3 direction);

	glm::vec3 getGlobalAmbient	(){ return _globalAmbient;  }
	glm::vec3 getGlobalDiffuse	(){ return _globalDiffuse;  }
	glm::vec3 getGlobalSpecular (){ return _globalSpecular; }
	glm::vec3 getGlobalDirection(){ return _globalDirection;}
	
	std::string* getCubemapFiles(){return _cubemapFiles;}
	void clear();

	// To do with light. SetShaderMap allows access to all loaded shaders. Not sure I like this design too much but ok for now
	// Note we would want a system that will set current global uniforms to any new shaders loaded in as well. perhaps we need a "shader manager" class?
	void setShadersMap(const std::map<std::string, Shader*>* shadersMap) { _loadedShaders = shadersMap; }
	void activateLights();
	

private:

	glm::vec3 _globalAmbient;	//!< Global Lighting - Ambient
	glm::vec3 _globalDiffuse;	//!< Global Lighting - Diffuse
	glm::vec3 _globalSpecular;  //!< Global Lighting - Specular
	glm::vec3 _globalDirection;	//!< Global Lighting - Direction

	std::vector<SPtr_Camera>   _cameras; //!< All cameras in scene. Currently set to simply render through camera at index 0.
	std::vector<SPtr_Renderer> _models; //!< All the models to be rendered each frame

	std::vector<SPtr_Renderer> _particles; //!< All the particle systems to be renderered each frame, these are done after all other renderering to allow for correct transparency

	std::vector<SPtr_Animator> _animators; //!< All the anims

	std::vector<SPtr_GUI> _gui;

	std::vector<SPtr_Light>    _pointLights; //!< All the lights in the scene
	std::vector<SPtr_Light>    _spotLights;
	std::vector<SPtr_Light>    _unsortedLights;
	// NOTE: Having a pointer to the map of assets isn't that elegant but it'll do for the moment. It's relatively safe as the map exists for the duration of the program (no null pointers) and quicker than cycling throgh materials (which may have the same shader - why update uniforms in the same shader more than once??)
	const std::map<std::string, Shader*>* _loadedShaders; //!< Pointer to map of shaders in assets. Rendering system needs to be able to update uniforms in all shaders (e.g. for lighting).

	//USED SKYBOX STUFF
	GLuint _skyboxTexture;
	std::string _cubemapFiles[Cubemap::SIDES];
	sf::Image _cubemapTexs[Cubemap::SIDES];
	GLuint _skyboxVAO, _skyboxVBO;
	Shader* _skyboxShader;

	//UNUSED SHADOWMAP STUFF
	GLuint _pass1Index, _pass2Index, _shadowFBO;
	glm::mat4 _lightPV ,_shadowView, _shadowModel, _shadowProjection, _shadowMapBias;
	int _shadowMapWidth, _shadowMapHeight;


};

#endif