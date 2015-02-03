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

#define DEMO_SCENE_PATH "demo.xml"


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

	void initialize(ObjectManager &objMngr, BehaviourSystem &behvrSys);
	
	void writeDemoXML(); //!< Write a basic scene to XML, mainly used for testing purposes.
	void loadFromXML(std::string filePath); //!< Load init table from xml file
	void initFromInitTable(); //!< Initialize all components to their starting values
	

private:
	ObjectManager* _objMngr;

	// functions to do with writing different types of info to file
	TiXmlElement* xmlAddGo(TiXmlDocument* doc, std::string name);
	void xmlAddTransform(TiXmlElement* go, glm::vec3 t, glm::vec3 r, glm::vec3 s);
	void xmlAddCamera(TiXmlElement* go);
	void xmlAddModelRend(TiXmlElement* go, PrimitiveShapes::Type mesh, std::string shader, std::string texture);
	void xmlAddModelRend(TiXmlElement* go, PrimitiveShapes::Type mesh, std::string shader, std::string texture, float tileU, float tileV);
	void xmlAddRobot(TiXmlElement* go);
	void xmlAddPhysBody(TiXmlElement* go);
	void xmlAddBehaviour(TiXmlElement* go, BehaviourTypes::Type type);


	// Functions that init a component
	void initTransform(CompData &comp);
	void initCamera(CompData &comp);
	void initModelRend(CompData &comp);
	void initRobot(CompData &comp);
	void initPhysBody(CompData &comp);
	void initLight(CompData &comp);
};

#endif