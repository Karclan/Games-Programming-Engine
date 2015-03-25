#include "core\sceneManager.h"

void SceneManager::initialize(ObjectManager &objMngr, RenderSystem &rendSys)
{
	_objMngr = &objMngr;
	_rendSys = &rendSys;
	//writeDemoXML(); // This writes data to the example XML file, note will overwirte demo.xml. Used for testing save functions
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
	}
	_rendSys->activateLights();
}


//------------------------------------------------------------------
//------------------------------------------------------------------

// clear scene
void SceneManager::clearScene()
{
	_objMngr->clearInitTable();
	_objMngr->destroyAll();
	_rendSys->setLightDefaults();
}

// load from XML
void SceneManager::loadFromXML(std::string filePath)
{
	// Ensure file name is ok
	filePath = ASSETS_PATH + "scenes/" + filePath;

	TiXmlDocument doc(filePath);
	if(!doc.LoadFile())
	{
		filePath = filePath + std::string(".XML");
		doc.SetValue(filePath);
	}


	if(!doc.LoadFile())
	{
		std::cout << "Failed to load XML!\n\n\n";
		return;
	}

	// Clear out old scene
	clearScene();

	TiXmlHandle handle(&doc);
	
	// Scene Params
	TiXmlElement* sceneElmnt = handle.FirstChildElement("SCENE").ToElement();
	if(sceneElmnt)
	{
		TiXmlElement* lightElmnt = sceneElmnt->FirstChildElement("LIGHTS");
	
		double x, y, z;

		TiXmlElement* lightParam = lightElmnt->FirstChildElement("AMB");
		if(lightParam)
		{
			lightParam->Attribute("r", &x);
			lightParam->Attribute("g", &y);
			lightParam->Attribute("b", &z);
			_rendSys->setGlobalAmbient(glm::vec3(x, y, z));
		}

		lightParam = lightElmnt->FirstChildElement("DIFF");
		if(lightParam)
		{
			lightParam->Attribute("r", &x);
			lightParam->Attribute("g", &y);
			lightParam->Attribute("b", &z);
			_rendSys->setGlobalDiffuse(glm::vec3(x, y, z));
		}

		if(lightParam)
		{
			lightParam = lightElmnt->FirstChildElement("SPEC");
			lightParam->Attribute("r", &x);
			lightParam->Attribute("g", &y);
			lightParam->Attribute("b", &z);
			_rendSys->setGlobalSpecular(glm::vec3(x, y, z));
		}

		if(lightParam)
		{
			lightParam = lightElmnt->FirstChildElement("DIR");
			lightParam->Attribute("x", &x);
			lightParam->Attribute("y", &y);
			lightParam->Attribute("z", &z);
			_rendSys->setGlobalDirection(glm::vec3(x, y, z));
		}
	}


	// Game Objects
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
	}

	// Initialize the scene
	initFromInitTable();
}





//------------------------------------------------------------------
//------------------------------------------------------------------
void SceneManager::saveToXML(std::string filePath)
{
	// Ensure file name is ok
	std::size_t dotPos =  filePath.find_first_of('.');
	if(dotPos != std::string::npos) filePath = filePath.substr(0, dotPos); // remove file extension if any specified
	filePath = ASSETS_PATH + "scenes/" + filePath + std::string(".XML"); // add correct file extension


	// Ref to demo file
	TiXmlDocument doc(filePath); // xml file

	// Declaration at start of xml file
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" ); // version declaration at top of file
	doc.LinkEndChild( decl ); // Add declaration to file, this auto cleans up pointer as well

	// Scene parametres
	TiXmlElement* sceneElmnt = new TiXmlElement("SCENE");
	doc.LinkEndChild(sceneElmnt);
	xmlAddSceneLights(sceneElmnt, _rendSys->getGlobalAmbient(), _rendSys->getGlobalDiffuse(), _rendSys->getGlobalSpecular(), _rendSys->getGlobalDirection());

	// Init table (Game Objects)
	InitTable* init = _objMngr->getInitTable();
	InitTableIterator it = init->begin();

	for(it; it != init->end(); ++it) // foreach game object in init table
	{
		std::string name = _objMngr->getGameObject(it->first)->getName();
		TiXmlElement * go = xmlAddGo(&doc, name);
		std::list<CompData>::iterator compData = it->second.components.begin();


		for(compData; compData != it->second.components.end(); ++compData)
		{
			switch(compData->getComp()->getType())
			{
			case ComponentType::TRANSFORM: 
				xmlAddTransform(go, glm::vec3(compData->getFloatAttrib(0), compData->getFloatAttrib(1), compData->getFloatAttrib(2)),
									glm::vec3(compData->getFloatAttrib(3), compData->getFloatAttrib(4), compData->getFloatAttrib(5)),
									glm::vec3(compData->getFloatAttrib(6), compData->getFloatAttrib(7), compData->getFloatAttrib(8)));
				break;

			case ComponentType::MODL_REND:
				{				
				xmlAddModelRend(
					go, 
					compData->getStringAttrib(0), 
					compData->getStringAttrib(1), 
					compData->getStringAttrib(2), //Texture Diffuse
					compData->getStringAttrib(3), //Texture Specular
					compData->getStringAttrib(4), //Texture Normal
					compData->getStringAttrib(5), //Texture Height
					glm::vec3 (compData->getFloatAttrib(8), compData->getFloatAttrib(9), compData->getFloatAttrib(10)), 
					glm::vec3 (compData->getFloatAttrib(11), compData->getFloatAttrib(12), compData->getFloatAttrib(13)), 
					compData->getFloatAttrib(14), 
					compData->getFloatAttrib(6), 
					compData->getFloatAttrib(7)
					);
				}
				break;

			case ComponentType::CAMERA:
				xmlAddCamera(go);
				break;

			case ComponentType::ROB_REND: 
				xmlAddRobot(go);
				break;

			case ComponentType::PARTICLE_REND:
				xmlAddParticleRend(go, 
					compData->getIntAttrib(0),   //Particle Pool Size
					compData->getIntAttrib(1),   //Box Position Generator
						glm::vec4(compData->getFloatAttrib(2),compData->getFloatAttrib(3),compData->getFloatAttrib(4),compData->getFloatAttrib(5)),
					compData->getIntAttrib(6),   //Round Position Generator
						compData->getFloatAttrib(7), //radiusX
						compData->getFloatAttrib(8), //radiusY
					compData->getIntAttrib(9),  //Basic Colour Generator
						glm::vec4(compData->getFloatAttrib(10),compData->getFloatAttrib(11),compData->getFloatAttrib(12),compData->getFloatAttrib(13)),
						glm::vec4(compData->getFloatAttrib(14),compData->getFloatAttrib(15),compData->getFloatAttrib(16),compData->getFloatAttrib(17)),
						glm::vec4(compData->getFloatAttrib(18),compData->getFloatAttrib(19),compData->getFloatAttrib(20),compData->getFloatAttrib(21)),
						glm::vec4(compData->getFloatAttrib(22),compData->getFloatAttrib(23),compData->getFloatAttrib(27),compData->getFloatAttrib(25)),
					compData->getIntAttrib(26),  //Basic Velocity Generator
						glm::vec4(compData->getFloatAttrib(27),compData->getFloatAttrib(28),compData->getFloatAttrib(29),compData->getFloatAttrib(30)),
						glm::vec4(compData->getFloatAttrib(31),compData->getFloatAttrib(32),compData->getFloatAttrib(33),compData->getFloatAttrib(34)),
					compData->getIntAttrib(35),  //Sphere Velocity Generator
						compData->getFloatAttrib(36),
						compData->getFloatAttrib(37),
					compData->getIntAttrib(38),  //Basic Time Generator
						compData->getFloatAttrib(39),
						compData->getFloatAttrib(40),
					compData->getIntAttrib(41),  //Euler updater
						glm::vec4(compData->getFloatAttrib(42),compData->getFloatAttrib(43),compData->getFloatAttrib(44),compData->getFloatAttrib(45)),
					compData->getIntAttrib(46),  //Attractor Updater	
						compData->getIntAttrib(47),
						glm::vec4(compData->getFloatAttrib(48),compData->getFloatAttrib(49),compData->getFloatAttrib(50),compData->getFloatAttrib(51)),
						compData->getIntAttrib(52),
						glm::vec4(compData->getFloatAttrib(53),compData->getFloatAttrib(54),compData->getFloatAttrib(55),compData->getFloatAttrib(56)),
						compData->getIntAttrib(57),
						glm::vec4(compData->getFloatAttrib(58),compData->getFloatAttrib(59),compData->getFloatAttrib(60),compData->getFloatAttrib(61)),
						compData->getIntAttrib(62),
						glm::vec4(compData->getFloatAttrib(63),compData->getFloatAttrib(64),compData->getFloatAttrib(65),compData->getFloatAttrib(66)),
					compData->getIntAttrib(67),  //Basic Colour Updater
					compData->getIntAttrib(68),  //Position Colour Updater
						glm::vec4(compData->getFloatAttrib(69),compData->getFloatAttrib(70),compData->getFloatAttrib(71),compData->getFloatAttrib(72)),
						glm::vec4(compData->getFloatAttrib(73),compData->getFloatAttrib(74),compData->getFloatAttrib(75),compData->getFloatAttrib(76)),
					compData->getIntAttrib(77),  //Velocity Colour Updater
						glm::vec4(compData->getFloatAttrib(78),compData->getFloatAttrib(79),compData->getFloatAttrib(80),compData->getFloatAttrib(81)),
						glm::vec4(compData->getFloatAttrib(82),compData->getFloatAttrib(83),compData->getFloatAttrib(84),compData->getFloatAttrib(85)),
					compData->getIntAttrib(86),  //Timer Updater
					compData->getFloatAttrib(87) //Emitter emit scalar
					);

				break;

			case ComponentType::PHY_BODY:
				xmlAddPhysBody(go);
				break;

			case ComponentType::LIGHT:
				xmlAddLight(go, compData->getIntAttrib(0), glm::vec3(compData->getFloatAttrib(1), compData->getFloatAttrib(2), compData->getFloatAttrib(3)),
														   glm::vec3(compData->getFloatAttrib(4), compData->getFloatAttrib(5), compData->getFloatAttrib(6)),
														   glm::vec3(compData->getFloatAttrib(7), compData->getFloatAttrib(8), compData->getFloatAttrib(9)));
				break;

			case ComponentType::SPHERE_COL:
				xmlAddSphereCol(go, compData->getFloatAttrib(0), glm::vec3(compData->getFloatAttrib(1), compData->getFloatAttrib(2), compData->getFloatAttrib(3)));
				break;

			case ComponentType::BOX_COL: 
				xmlAddBoxCol(go, glm::vec3(compData->getFloatAttrib(0), compData->getFloatAttrib(1), compData->getFloatAttrib(2)),
								 glm::vec3(compData->getFloatAttrib(3), compData->getFloatAttrib(4), compData->getFloatAttrib(5)));
				break;

			case ComponentType::CUSTOM:
				xmlAddCustom(go, compData->getStringAttrib(0));
				break;
			}
		}
	}

	
	// Save doc
	if(doc.SaveFile()) std::cout << "Saved to " << filePath << "!\n";
	else
		std::cout << "Save Failed. Ensure you have a folder called 'scenes' in the assets folder\n";
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
void SceneManager::xmlAddModelRend(TiXmlElement* go, std::string mesh, std::string shader, std::string textureD, std::string textureS, std::string textureN, std::string textureH, glm::vec3 diff, glm::vec3 spec, float specExp) {xmlAddModelRend(go, mesh, shader, textureD,textureS,textureN,textureH, diff, spec, specExp, 1, 1);}
void SceneManager::xmlAddModelRend(TiXmlElement* go, std::string mesh, std::string shader, std::string textureD, std::string textureS, std::string textureN, std::string textureH, glm::vec3 diff, glm::vec3 spec, float specExp, float tileU, float tileV)
{
	TiXmlElement* transElmnt = new TiXmlElement("COMP"); // Component Element
	transElmnt->SetAttribute("type", ComponentType::MODL_REND); // Set type attrib
	transElmnt->SetAttribute("mesh", mesh);			   // 0 Set mesh attrib
	transElmnt->SetAttribute("shader", shader);		   // 1 Set material attrib

	transElmnt->SetAttribute("textureD", textureD);     // 2 Set texture attrib DIFFUSE
	transElmnt->SetAttribute("textureS", textureS);     // 3 Set texture attrib SPECULAR
	transElmnt->SetAttribute("textureN", textureN);     // 4 Set texture attrib NORMAL
	transElmnt->SetAttribute("textureH", textureH);     // 5 Set texture attrib HEIGTH
													   
	transElmnt->SetDoubleAttribute("tileU", tileU);    // 6 Set tile U attrib
	transElmnt->SetDoubleAttribute("tileV", tileV);    // 7 Set tile U attrib
	transElmnt->SetDoubleAttribute("dR", diff.x);      // 8 diff
	transElmnt->SetDoubleAttribute("dG", diff.y);      // 9 diff
	transElmnt->SetDoubleAttribute("dB", diff.z);      // 10 diff
	transElmnt->SetDoubleAttribute("sR", spec.x);      // 11 spec
	transElmnt->SetDoubleAttribute("sG", spec.y);      // 12 spec
	transElmnt->SetDoubleAttribute("sB", spec.z);      // 13 spec
	transElmnt->SetDoubleAttribute("specEx", specExp); // 14 spec exp

	go->LinkEndChild(transElmnt); // Add element to file, this auto cleans up pointer as well
}

void SceneManager::xmlAddLight(TiXmlElement* go, int type, glm::vec3 diff, glm::vec3 spec, glm::vec3 atten)
{
	TiXmlElement* lightElmnt = new TiXmlElement("COMP"); // Component Element
	lightElmnt->SetAttribute("type", ComponentType::LIGHT); // Set type attrib
	lightElmnt->SetAttribute("lightType", type);  // 0 light type (0=dir, 1=point, 2=spot)
	lightElmnt->SetDoubleAttribute("dR", diff.x); // 1 Diffuse x
	lightElmnt->SetDoubleAttribute("dG", diff.y); // 2 Diffuse y
	lightElmnt->SetDoubleAttribute("dB", diff.z); // 3 Diffuse z
	lightElmnt->SetDoubleAttribute("sR", spec.x); // 4 Specular x
	lightElmnt->SetDoubleAttribute("sG", spec.y); // 5 Specular y
	lightElmnt->SetDoubleAttribute("sB", spec.z); // 6 Specular z
	lightElmnt->SetDoubleAttribute("constant", atten.x); // 7 Constant
	lightElmnt->SetDoubleAttribute("linear", atten.y); // 8 Linear
	lightElmnt->SetDoubleAttribute("quadratic", atten.z); // 9 Quadratic

	go->LinkEndChild(lightElmnt); // Add element to file, this auto cleans up pointer as well
}

void SceneManager::xmlAddParticleRend(TiXmlElement* go, 
		int poolSize, 
/*X*/	bool boxPosGenActive,	glm::vec4 boxPosOffset, 
/*X*/	bool roundPosGenActive, float radiusX, float radiusY,
/*X*/	bool bColourGenActive,	glm::vec4 bMinStCol, glm::vec4 bMaxStCol, glm::vec4 bMinEndCol, glm::vec4 bMaxEndCol, 
/*X*/	bool bVelGenActive,		glm::vec4 bMinVel,   glm::vec4 bMaxVel, 
/*X*/	bool bSphereGenActive,	float bSMinVel, float bSMaxVel,
/*X*/	bool bTimeGenActive,	float minTime, float maxTime,
/*X*/	bool eulUpdActive,		glm::vec4 globalAcc,
/*X*/	bool attUpdActive,		bool att1Act, glm::vec4 att1, 
								bool att2Act, glm::vec4 att2, 
								bool att3Act, glm::vec4 att3, 
								bool att4Act, glm::vec4 att4,
/*X*/	bool bColUpdActive,
/*X*/	bool pColUpdActive,		glm::vec4 minPosCol, glm::vec4 maxPosCol,
/*X*/	bool velColUpdActive,   glm::vec4 minVelCol, glm::vec4 maxVelCol,
/*X*/	bool tUpdActive,
/*X*/   float emitScalar)
{
	TiXmlElement* particleElement = new TiXmlElement("COMP");
	particleElement->SetAttribute("type",ComponentType::PARTICLE_REND);

	particleElement->SetAttribute("poolSize",poolSize);

	//Box Position Generator
	particleElement->SetAttribute("boxPosGenActive", boxPosGenActive);
	particleElement->SetDoubleAttribute("boxPosOffX",boxPosOffset.x);
	particleElement->SetDoubleAttribute("boxPosOffY",boxPosOffset.y);
	particleElement->SetDoubleAttribute("boxPosOffZ",boxPosOffset.z);
	particleElement->SetDoubleAttribute("boxPosOffW",boxPosOffset.w);
	//Round Position Generator
	particleElement->SetAttribute("roundPosGenActive", roundPosGenActive);
	particleElement->SetDoubleAttribute("radiusX",radiusX);
	particleElement->SetDoubleAttribute("radiusY",radiusY);

	//Basic Colour Generator
	particleElement->SetAttribute("bColourGenActive", bColourGenActive);
	particleElement->SetDoubleAttribute("bMinStColX",bMinStCol.x);
	particleElement->SetDoubleAttribute("bMinStColY",bMinStCol.y);
	particleElement->SetDoubleAttribute("bMinStColZ",bMinStCol.z);
	particleElement->SetDoubleAttribute("bMinStColW",bMinStCol.w);

	particleElement->SetDoubleAttribute("bMaxStColX",bMaxStCol.x);
	particleElement->SetDoubleAttribute("bMaxStColY",bMaxStCol.y);
	particleElement->SetDoubleAttribute("bMaxStColZ",bMaxStCol.z);
	particleElement->SetDoubleAttribute("bMaxStColW",bMaxStCol.w);

	particleElement->SetDoubleAttribute("bMinEndColX",bMinEndCol.x);
	particleElement->SetDoubleAttribute("bMinEndColY",bMinEndCol.y);
	particleElement->SetDoubleAttribute("bMinEndColZ",bMinEndCol.z);
	particleElement->SetDoubleAttribute("bMinEndColW",bMinEndCol.w);

	particleElement->SetDoubleAttribute("bMaxEndColX",bMaxEndCol.x);
	particleElement->SetDoubleAttribute("bMaxEndColY",bMaxEndCol.y);
	particleElement->SetDoubleAttribute("bMaxEndColZ",bMaxEndCol.z);
	particleElement->SetDoubleAttribute("bMaxEndColW",bMaxEndCol.w);
	//Basic Velocity Generator
	particleElement->SetAttribute("bVelGenActive", bVelGenActive);
	particleElement->SetDoubleAttribute("bMinVelX",bMinVel.x);
	particleElement->SetDoubleAttribute("bMinVelY",bMinVel.y);
	particleElement->SetDoubleAttribute("bMinVelZ",bMinVel.z);
	particleElement->SetDoubleAttribute("bMinVelW",bMinVel.w);

	particleElement->SetDoubleAttribute("bMaxVelX",bMaxVel.x);
	particleElement->SetDoubleAttribute("bMaxVelY",bMaxVel.y);
	particleElement->SetDoubleAttribute("bMaxVelZ",bMaxVel.z);
	particleElement->SetDoubleAttribute("bMaxVelW",bMaxVel.w);
	//Basic Sphere Generator
	particleElement->SetAttribute("bSphereGenActive", bSphereGenActive);
	particleElement->SetDoubleAttribute("bSMinVelX",bSMinVel);
	particleElement->SetDoubleAttribute("bSMaxVelX",bSMaxVel);

	//Basic Time Generator
	particleElement->SetAttribute("bTimeGenActive", bTimeGenActive);
	particleElement->SetAttribute("minTime", minTime);
	particleElement->SetAttribute("maxTime", maxTime);
	//Euler Updater
	particleElement->SetAttribute("eulUpdActive", eulUpdActive);
	particleElement->SetDoubleAttribute("globalAccX",globalAcc.x);
	particleElement->SetDoubleAttribute("globalAccY",globalAcc.y);
	particleElement->SetDoubleAttribute("globalAccZ",globalAcc.z);
	particleElement->SetDoubleAttribute("globalAccW",globalAcc.w);
	//Attractor Updater
	particleElement->SetAttribute("attUpdActive", attUpdActive);
	particleElement->SetAttribute("att1Act", att1Act);
	particleElement->SetDoubleAttribute("att1X",att1.x);
	particleElement->SetDoubleAttribute("att1Y",att1.y);
	particleElement->SetDoubleAttribute("att1Z",att1.z);
	particleElement->SetDoubleAttribute("att1W",att1.w);

	particleElement->SetAttribute("att2Act", att2Act);
	particleElement->SetDoubleAttribute("att2X",att2.x);
	particleElement->SetDoubleAttribute("att2Y",att2.y);
	particleElement->SetDoubleAttribute("att2Z",att2.z);
	particleElement->SetDoubleAttribute("att2W",att2.w);

	particleElement->SetAttribute("att3Act", att3Act);
	particleElement->SetDoubleAttribute("att3X",att3.x);
	particleElement->SetDoubleAttribute("att3Y",att3.y);
	particleElement->SetDoubleAttribute("att3Z",att3.z);
	particleElement->SetDoubleAttribute("att3W",att3.w);

	particleElement->SetAttribute("att4Act", att4Act);
	particleElement->SetDoubleAttribute("att4X",att4.x);
	particleElement->SetDoubleAttribute("att4Y",att4.y);
	particleElement->SetDoubleAttribute("att4Z",att4.z);
	particleElement->SetDoubleAttribute("att4W",att4.w);
	//Basic Colour Updater
	particleElement->SetAttribute("bColUpdActive", bColUpdActive);
	//Position Colour Updater
	particleElement->SetAttribute("pColUpdActive", pColUpdActive);

	particleElement->SetDoubleAttribute("minPosColX",minPosCol.x);
	particleElement->SetDoubleAttribute("minPosColY",minPosCol.y);
	particleElement->SetDoubleAttribute("minPosColZ",minPosCol.z);
	particleElement->SetDoubleAttribute("minPosColW",minPosCol.w);

	particleElement->SetDoubleAttribute("maxPosColX",maxPosCol.x);
	particleElement->SetDoubleAttribute("maxPosColY",maxPosCol.y);
	particleElement->SetDoubleAttribute("maxPosColZ",maxPosCol.z);
	particleElement->SetDoubleAttribute("maxPosColW",maxPosCol.w);
	//Velocity Colour Updater
	particleElement->SetAttribute("velColUpdActive", velColUpdActive);

	particleElement->SetDoubleAttribute("minVelColX",minVelCol.x);
	particleElement->SetDoubleAttribute("minVelColY",minVelCol.y);
	particleElement->SetDoubleAttribute("minVelColZ",minVelCol.z);
	particleElement->SetDoubleAttribute("minVelColW",minVelCol.w);

	particleElement->SetDoubleAttribute("maxVelColX",maxVelCol.x);
	particleElement->SetDoubleAttribute("maxVelColY",maxVelCol.y);
	particleElement->SetDoubleAttribute("maxVelColZ",maxVelCol.z);
	particleElement->SetDoubleAttribute("maxVelColW",maxVelCol.w);
	//Timer Updater
	particleElement->SetAttribute("tUpdActive", tUpdActive);
	//Emitter
	particleElement->SetAttribute("emitScalar",emitScalar);

	go->LinkEndChild(particleElement);
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

void SceneManager::xmlAddCustom(TiXmlElement* go, std::string behvr)
{
	TiXmlElement* boxColElmnt = new TiXmlElement("COMP"); // Component Element
	boxColElmnt->SetAttribute("type", ComponentType::CUSTOM); // Set type attrib
	boxColElmnt->SetAttribute("behvr", behvr);
	go->LinkEndChild(boxColElmnt); // Add element to file, this auto cleans up pointer as well

}





//=======SCENE PARAM FUNCS
void SceneManager::xmlAddSceneLights(TiXmlElement* scene, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir)
{
	TiXmlElement* lightsElmnt = new TiXmlElement("LIGHTS");
	scene->LinkEndChild(lightsElmnt);

	TiXmlElement* ambElmnt = new TiXmlElement("AMB");
	ambElmnt->SetDoubleAttribute("r", amb.x);
	ambElmnt->SetDoubleAttribute("g", amb.y);
	ambElmnt->SetDoubleAttribute("b", amb.z);
	lightsElmnt->LinkEndChild(ambElmnt);

	TiXmlElement* diffElmnt = new TiXmlElement("DIFF");
	diffElmnt->SetDoubleAttribute("r", diff.x);
	diffElmnt->SetDoubleAttribute("g", diff.y);
	diffElmnt->SetDoubleAttribute("b", diff.z);
	lightsElmnt->LinkEndChild(diffElmnt);

	TiXmlElement* specElmnt = new TiXmlElement("SPEC");
	specElmnt->SetDoubleAttribute("r", spec.x);
	specElmnt->SetDoubleAttribute("g", spec.y);
	specElmnt->SetDoubleAttribute("b", spec.z);
	lightsElmnt->LinkEndChild(specElmnt);

	TiXmlElement* dirElmnt = new TiXmlElement("DIR");
	dirElmnt->SetDoubleAttribute("x", amb.x);
	dirElmnt->SetDoubleAttribute("y", amb.y);
	dirElmnt->SetDoubleAttribute("z", amb.z);
	lightsElmnt->LinkEndChild(dirElmnt);
}

// END SAVING FUNCTIONS------------------------------------------------------------------