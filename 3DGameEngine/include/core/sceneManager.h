#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <string>
#include <vector>

#include "tinyXML\tinystr.h"
#include "tinyXML\tinyxml.h"

#include "core\initTable.h"
#include "core\objectManager.h"
#include "behaviour\behaviourSystem.h"
#include "core\assets.h"
#include "rendering\robotRenderer.h"
#include "physics\physicsBody.h"




/*! \brief Responsible for loading scenes

	Scenes are loaded from XML into local memory as data objects in 
	the init table. This is the used to generate and initialize
	game objects.
*/
class SceneManager
{
public:
	SceneManager(){};
	~SceneManager(){};

	void initialize(ObjectManager &objMngr, RenderSystem &rendSys);
	
	void clearScene(); //!< Clear current scene to start a new, blank scene
	void loadFromXML(std::string filePath); //!< Load init table from xml file
	void initFromInitTable(); //!< Initialize all components to their starting values
	void saveToXML(std::string filePath); //!< Save from init table to XML

private:
	ObjectManager* _objMngr;
	RenderSystem* _rendSys;

	// functions to do with writing different types of info to file
	TiXmlElement* xmlAddGo(TiXmlDocument* doc, std::string name);
	void xmlAddTransform(TiXmlElement* go, glm::vec3 t, glm::vec3 r, glm::vec3 s);
	void xmlAddCamera(TiXmlElement* go);
	void xmlAddModelRend(TiXmlElement* go, std::string mesh, std::string shader, std::string texture, glm::vec3 diff, glm::vec3 spec, float specExp);
	void xmlAddModelRend(TiXmlElement* go, std::string mesh, std::string shader, std::string texture, glm::vec3 diff, glm::vec3 spec, float specExp, float tileU, float tileV);
	void xmlAddRobot(TiXmlElement* go);
	void xmlAddPhysBody(TiXmlElement* go);
	void xmlAddSphereCol(TiXmlElement* go, float radius, glm::vec3 offset);
	void xmlAddBoxCol(TiXmlElement* go, glm::vec3 extents, glm::vec3 offset);
	void xmlAddLight(TiXmlElement* go, int type, glm::vec3 diff, glm::vec3 spec, glm::vec3 atten);
	void xmlAddCustom(TiXmlElement* go, std::string behvr);
	void xmlAddAnimator(TiXmlElement* go, std::string animationPath);
	void xmlAddTerrainCol(TiXmlElement* go);

	// saving scene params funcs
	void xmlAddSceneLights(TiXmlElement* scene, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir);
	
};

#endif