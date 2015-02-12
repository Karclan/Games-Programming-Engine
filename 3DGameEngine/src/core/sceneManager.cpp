#include "core\sceneManager.h"

void SceneManager::initialize(ObjectManager &objMngr, BehaviourSystem &behvrSys)
{
	_objMngr = &objMngr;
	writeDemoXML(); // This writes data to the example XML file, note will overwirte demo.xml. Used for testing save functions
}

void SceneManager::initFromInitTable()
{
	InitTable* initTable = _objMngr->getInitTable();
	InitTableIterator go;
	for(go = initTable->begin(); go != initTable->end(); ++go)
	{
		// Iterate through every component and init it
		for(std::list<CompData>::iterator comp = go->second.components.begin(); comp != go->second.components.end(); ++comp)
		{
			comp->initializeComponent();
		}

		// Iterate through behaviours and init them
		for(int i = 0 ; i < go->second.behaviours.size(); ++i)
		{
			go->second.behaviours[i]->reset();
		}
	}
}


//------------------------------------------------------------------
//------------------------------------------------------------------


// load from XML
void SceneManager::loadFromXML(std::string filePath)
{
	TiXmlDocument doc(filePath);
	if(!doc.LoadFile())
	{
		std::cout << "Failed to load XML!\n\n\n";
		return;
	}

	// Clear out old scene
	_objMngr->clearInitTable();
	_objMngr->destroyAll();

	TiXmlHandle handle(&doc);
	
	TiXmlElement* goElmnt;
	goElmnt = handle.FirstChildElement("GO").ToElement();
	for(goElmnt; goElmnt != NULL; goElmnt=goElmnt->NextSiblingElement()) // foreach game object element in xml
	{
		// Get Name
		std::string goName;
		if(goElmnt->Attribute("name"))
			goName = goElmnt->Attribute("name");
		else
			goName = goElmnt->Attribute("gameObject");

		// Create the game object and add to init table
		unsigned int goID = _objMngr->createGameObject(goName); // Create GO
		_objMngr->addComponentsFromXML(goID, goElmnt); // Add all componets via xml
		_objMngr->addBehavioursFromXML(goID, goElmnt);
	}
}





//------------------------------------------------------------------
//------------------------------------------------------------------










// Writes an example scene to XML. Mainly used for testing
void SceneManager::writeDemoXML()
{
	// Ref to demo file
	TiXmlDocument doc(DEMO_SCENE_PATH); // xml file

	// Declaration at start of xml file
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" ); // version declaration at top of file
	doc.LinkEndChild( decl ); // Add declaration to file, this auto cleans up pointer as well

	//------------------- For platform game demo
	// Robot
	TiXmlElement * robot = xmlAddGo(&doc, "Robot");
	xmlAddPhysBody(robot);
	xmlAddSphereCol(robot, 1, glm::vec3(0, 0, 0));
	//xmlAddBoxCol(robot, glm::vec3(5, 12, 2), glm::vec3(0, -1.5, 0));
	xmlAddBehaviour(robot, BehaviourTypes::PLAYER_CON);
	xmlAddTransform(robot, glm::vec3(0, 0.8f, 0), glm::vec3(), glm::vec3(0.1f, 0.1f, 0.1f));
	xmlAddRobot(robot);

	// Random test collidy thing
	TiXmlElement * ball = xmlAddGo(&doc, "Ball");
	xmlAddModelRend(ball, "bs_ears.obj", "advanced", "ogre_diffuse.png");
	xmlAddPhysBody(ball);
	//xmlAddSphereCol(ball, 1.0f, glm::vec3());
	xmlAddBoxCol(ball, glm::vec3(1, 2, 1), glm::vec3(0, 0, 0));
	xmlAddTransform(ball, glm::vec3(1.5f, 1, 0), glm::vec3(), glm::vec3(1, 1, 1));


	// Camera Object
	TiXmlElement * cameraGo = xmlAddGo(&doc, "Camera");
	xmlAddTransform(cameraGo, glm::vec3(0, 0, 5), glm::vec3(), glm::vec3(1, 1, 1));
	xmlAddCamera(cameraGo);

	// Floor
	TiXmlElement * floor = xmlAddGo(&doc, "Floor");
	xmlAddTransform(floor, glm::vec3(0, -0.005f, 0), glm::vec3(), glm::vec3(40, 0.01f, 40));
	xmlAddModelRend(floor, PrimitiveShapes::CUBE, "advanced", "grass.png", 20, 20);

	// Walls
	TiXmlElement * wall = xmlAddGo(&doc, "Wall");
	xmlAddTransform(wall, glm::vec3(0, 0.3, -2), glm::vec3(), glm::vec3(10, 0.5f, 0.5f));
	xmlAddModelRend(wall, PrimitiveShapes::CUBE, "advanced", "wall.png", 20, 1);

	TiXmlElement * wall2 = xmlAddGo(&doc, "Wall");
	xmlAddTransform(wall2, glm::vec3(-5.25, 0.3, 2.75), glm::vec3(0, 90, 0), glm::vec3(10, 0.5f, 0.5f));
	xmlAddModelRend(wall2, PrimitiveShapes::CUBE, "advanced", "wall.png", 20, 1);

	TiXmlElement * wall3 = xmlAddGo(&doc, "Wall");
	xmlAddTransform(wall3, glm::vec3(-10, 0.3, 8), glm::vec3(0, 0, 0), glm::vec3(10, 0.5f, 0.5f));
	xmlAddModelRend(wall3, PrimitiveShapes::CUBE, "advanced", "wall.png", 20, 1);


	// Windmills and sails
	glm::vec2 windPosits[] = { glm::vec2(-1, -5), glm::vec2(3, 0), glm::vec2(-6.5, 6) };
	for(int i = 0; i < 3; i++)
	{
		// Windmill
		TiXmlElement * windmill = xmlAddGo(&doc, "Windmill");
		xmlAddTransform(windmill, glm::vec3(windPosits[i].x, 1.3f, windPosits[i].y), glm::vec3(), glm::vec3(1, 2.6f, 1));
		xmlAddModelRend(windmill, PrimitiveShapes::CUBE, "advanced", "windmill.png", 1, 2.6f);

		// Sail
		TiXmlElement * sail = xmlAddGo(&doc, "Sail");
		xmlAddTransform(sail, glm::vec3(windPosits[i].x, 2, windPosits[i].y + 0.525f), glm::vec3(), glm::vec3(2, 2, 0.05f));
		xmlAddModelRend(sail, PrimitiveShapes::CUBE, "advanced", "sails.png");
		xmlAddBehaviour(sail, BehaviourTypes::ROT_OBJ);
	}

	

	
	// Save doc
	doc.SaveFile();
}





// SAVING FUNCTIONS -----------------------------------------------------------------------
// The following functions are to do with writing to XML and converting params given
// to correct text in XML file. Currentlt only called from writeDemoXML() but will be 
// part of dynamic file saving system later
//------------------------------------------------------------------------------------------

TiXmlElement* SceneManager::xmlAddGo(TiXmlDocument* doc, std::string name)
{
	TiXmlElement* goElement = new TiXmlElement("GO"); // Game Object Element
	goElement->SetAttribute("name", name); // Set GO name attrib
	doc->LinkEndChild( goElement ); // Add element to file, this auto cleans up pointer as well
	return goElement;
}

void SceneManager::xmlAddTransform(TiXmlElement* go, glm::vec3 t, glm::vec3 r, glm::vec3 s)
{
	TiXmlElement* transElmnt = new TiXmlElement("COMP"); // Component Element
	transElmnt->SetAttribute("type", ComponentType::TRANSFORM); // Set type attrib
	transElmnt->SetDoubleAttribute("tx", t.x); // Set translateX attrib
	transElmnt->SetDoubleAttribute("ty", t.y); // Set translateY attrib
	transElmnt->SetDoubleAttribute("tz", t.z); // Set translateZ attrib
	transElmnt->SetDoubleAttribute("rx", r.x); // Set rotateX attrib
	transElmnt->SetDoubleAttribute("ry", r.y); // Set rotateY attrib
	transElmnt->SetDoubleAttribute("rz", r.z); // Set rotateZ attrib
	transElmnt->SetDoubleAttribute("sx", s.x); // Set scaleX attrib
	transElmnt->SetDoubleAttribute("sy", s.y); // Set scaleY attrib
	transElmnt->SetDoubleAttribute("sz", s.z); // Set scaleZ attrib
	go->LinkEndChild(transElmnt); // Add element to file, this auto cleans up pointer as well
}

void SceneManager::xmlAddCamera(TiXmlElement* go)
{
	TiXmlElement* transElmnt = new TiXmlElement("COMP"); // Component Element
	transElmnt->SetAttribute("type", ComponentType::CAMERA); // Set type attrib
	go->LinkEndChild(transElmnt); // Add element to file, this auto cleans up pointer as well
}

// Add model renderer.
void SceneManager::xmlAddModelRend(TiXmlElement* go, PrimitiveShapes::Type mesh, std::string shader, std::string texture){xmlAddModelRend(go, mesh, shader, texture, 1, 1);}
void SceneManager::xmlAddModelRend(TiXmlElement* go, PrimitiveShapes::Type mesh, std::string shader, std::string texture, float tileU, float tileV)
{
	TiXmlElement* transElmnt = new TiXmlElement("COMP"); // Component Element
	transElmnt->SetAttribute("type", ComponentType::MODL_REND); // Set type attrib
	transElmnt->SetAttribute("primitive", (int)true); // It is a primitive mesh, not one loaded from file
	transElmnt->SetAttribute("mesh", mesh); // Set mesh attrib
	transElmnt->SetAttribute("shader", shader); // Set material attrib
	transElmnt->SetAttribute("texture", texture); // Set texture attrib
	transElmnt->SetDoubleAttribute("tileU", tileU); // Set tile U attrib
	transElmnt->SetDoubleAttribute("tileV", tileV); // Set tile U attrib
	go->LinkEndChild(transElmnt); // Add element to file, this auto cleans up pointer as well
}

void SceneManager::xmlAddModelRend(TiXmlElement* go, std::string mesh, std::string shader, std::string texture){xmlAddModelRend(go, mesh, shader, texture, 1, 1);}
void SceneManager::xmlAddModelRend(TiXmlElement* go, std::string mesh, std::string shader, std::string texture, float tileU, float tileV)
{
	TiXmlElement* transElmnt = new TiXmlElement("COMP"); // Component Element
	transElmnt->SetAttribute("type", ComponentType::MODL_REND); // Set type attrib
	transElmnt->SetAttribute("primitive", (int)false); // It is loaded from file
	transElmnt->SetAttribute("mesh", mesh); // Set mesh attrib
	transElmnt->SetAttribute("shader", shader); // Set material attrib
	transElmnt->SetAttribute("texture", texture); // Set texture attrib
	transElmnt->SetDoubleAttribute("tileU", tileU); // Set tile U attrib
	transElmnt->SetDoubleAttribute("tileV", tileV); // Set tile U attrib
	go->LinkEndChild(transElmnt); // Add element to file, this auto cleans up pointer as well
}

void SceneManager::xmlAddBehaviour(TiXmlElement* go, BehaviourTypes::Type type)
{
	TiXmlElement* behavElmnt = new TiXmlElement("BHVR"); // behaviour element
	behavElmnt->SetAttribute("type", type);
	go->LinkEndChild(behavElmnt);
}

void SceneManager::xmlAddRobot(TiXmlElement* go)
{
	TiXmlElement* robotElmnt = new TiXmlElement("COMP"); // Component Element
	robotElmnt->SetAttribute("type", ComponentType::ROB_REND); // Set type attrib
	go->LinkEndChild(robotElmnt); // Add element to file, this auto cleans up pointer as well
}

void SceneManager::xmlAddPhysBody(TiXmlElement* go)
{
	TiXmlElement* physBodyElmnt = new TiXmlElement("COMP"); // Component Element
	physBodyElmnt->SetAttribute("type", ComponentType::PHY_BODY); // Set type attrib
	go->LinkEndChild(physBodyElmnt); // Add element to file, this auto cleans up pointer as well
}


void SceneManager::xmlAddSphereCol(TiXmlElement* go, float radius, glm::vec3 offset)
{
	TiXmlElement* sphereColElmnt = new TiXmlElement("COMP"); // Component Element
	sphereColElmnt->SetAttribute("type", ComponentType::SPHERE_COL); // Set type attrib
	sphereColElmnt->SetDoubleAttribute("radius", radius);
	sphereColElmnt->SetDoubleAttribute("ox", offset.x);
	sphereColElmnt->SetDoubleAttribute("oy", offset.y);
	sphereColElmnt->SetDoubleAttribute("oz", offset.z);
	go->LinkEndChild(sphereColElmnt); // Add element to file, this auto cleans up pointer as well
}


void SceneManager::xmlAddBoxCol(TiXmlElement* go, glm::vec3 extents, glm::vec3 offset)
{
	TiXmlElement* boxColElmnt = new TiXmlElement("COMP"); // Component Element
	boxColElmnt->SetAttribute("type", ComponentType::BOX_COL); // Set type attrib
	boxColElmnt->SetDoubleAttribute("ex", extents.x);
	boxColElmnt->SetDoubleAttribute("ey", extents.y);
	boxColElmnt->SetDoubleAttribute("ez", extents.z);
	boxColElmnt->SetDoubleAttribute("ox", offset.x);
	boxColElmnt->SetDoubleAttribute("oy", offset.y);
	boxColElmnt->SetDoubleAttribute("oz", offset.z);
	go->LinkEndChild(boxColElmnt); // Add element to file, this auto cleans up pointer as well
}

// END SAVING FUNCTIONS------------------------------------------------------------------