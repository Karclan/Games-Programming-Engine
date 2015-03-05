#include "editor\goMenu.h"
#include <iostream>


void GoMenu::initialize(ObjectManager* objMngr, SceneManager* sceneMngr, EditorCamera* editorCam)
{
	_objectMngr = objMngr;
	_sceneMngr = sceneMngr;
	_editorCam = editorCam;
	testBool = false;
	saveFilePath = "";
	loadFilePath = "";
	_noAttribs = "No Attributes";
}

static void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
  // Copy the content of souceString handled by the AntTweakBar library to destinationClientString handled by your application
  destinationClientString = sourceLibraryString;
}

/******************************** Add Component Functions ********************************/

static void TW_CALL addBoxColComponent(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->addComponent(ComponentType::BOX_COL);
}

static void TW_CALL addCameraComponent(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->addComponent(ComponentType::CAMERA);
}

static void TW_CALL addLightComponent(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->addComponent(ComponentType::LIGHT);
}

static void TW_CALL addModelRenderComponent(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->addComponent(ComponentType::MODL_REND);
}

static void TW_CALL addPhysicsBodyComponent(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->addComponent(ComponentType::PHY_BODY);
}

static void TW_CALL addRobotRenderComponent(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->addComponent(ComponentType::ROB_REND);
}

static void TW_CALL addSphereColComponent(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->addComponent(ComponentType::SPHERE_COL);
}

static void TW_CALL addTransformComponent(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->addComponent(ComponentType::TRANSFORM);
}

static void TW_CALL addCustomComponent(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->addComponent(ComponentType::CUSTOM);
}

static void TW_CALL addTerrainColComponent(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->addComponent(ComponentType::TERRAIN_COL);
}

void GoMenu::addComponent(ComponentType::Type type)
{
	_objectMngr->addComponent(_selectedObjectID, type);
	refreshTweakBar();

	TwRefreshBar(_myBar);
	TwRefreshBar(_addCompBar);
	TwRefreshBar(_utilityBar);
}

/******************************** Save to File Functions ********************************/

static void TW_CALL  saveToFile(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->saveToFileXML();
}

void GoMenu::saveToFileXML()
{
	_sceneMngr->saveToXML(saveFilePath);
}


/******************************** Load from File Functions ********************************/

static void TW_CALL  loadFromFile(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->loadFromFileXML();
}

void GoMenu::loadFromFileXML()
{
	_sceneMngr->loadFromXML(loadFilePath);
	setSelectedObject(0); // important to set currently selected object to 0 as might be out of range!!
}


/******************************** New Level Function ********************************/

static void TW_CALL  newLevel(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->newScene();
}

void GoMenu::newScene()
{
	_sceneMngr->clearScene();
	setSelectedObject(0); // important to set currently selected object to 0 as might be out of range!!
}

/******************************** Create Game Object Function ********************************/

static void TW_CALL  makeGameObject(void *clientData)
{
	GoMenu* goMenu = (GoMenu*)clientData;

	goMenu->createGameObject();
}

void GoMenu::createGameObject()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode
	setSelectedObject(_objectMngr->createGameObject("New Game Object"));
	refreshTweakBar();
}


/******************************** Tweak Bars Setup ********************************/
void GoMenu::createTweakBar()
{
	_myBar = TwNewBar("Game Object");
	_addCompBar = TwNewBar("Add Component");
	_utilityBar = TwNewBar("Utility Bar");
	refreshTweakBar();
	TwCopyStdStringToClientFunc(CopyStdStringToClient); // CopyStdStringToClient implementation is given above
	setSelectedObject(0);

	// Write file path/save function
	TwAddVarRW(_utilityBar, "Save_File_Path_Name", TW_TYPE_STDSTRING, &saveFilePath, "group=SaveTo label=File_Path_Name");
	TwAddButton(_utilityBar, "Save", saveToFile, this, "group=SaveTo");
	TwAddVarRW(_utilityBar, "Load_File_Path_Name", TW_TYPE_STDSTRING, &loadFilePath, "group=LoadFrom label=File_Path_Name");
	TwAddButton(_utilityBar, "Load", loadFromFile, this, "group=LoadFrom");
	TwAddButton(_utilityBar, "Start New Level", newLevel, this, "group=NewLevel");
	TwAddButton(_utilityBar, "Create Game Object", makeGameObject, this, "group=CreateGameObject");
}


void GoMenu::refreshTweakBar()
{
	// Remove all the variables from the tweak bar
	TwRemoveAllVars(_myBar);

	// TwDefine(" GLOBAL fontstyle=fixed ");	// Changes the font style

	// Add the GO Name
	TwAddVarRW(_myBar, "NAME", TW_TYPE_STDSTRING, &_objName, NULL);
	TwAddVarRW(_addCompBar, "NAME", TW_TYPE_STDSTRING, &_objName, NULL);
	


	// Get game object name
	_objName = "No Object Selected";
	SPtr_GameObject go = _objectMngr->getGameObject(_selectedObjectID);
	if(go)
	{
		_objName = go->getName();

		InitTable* initTable = _objectMngr->getInitTable();
		GOData* goData = &initTable->find(_selectedObjectID)->second;

		//TwAddVarRW(_addCompBar, "test", TW_TYPE_BOOL8, &testBool, "");
		TwAddButton(_addCompBar, "addBoxCol", addBoxColComponent, this, "");
		TwAddButton(_addCompBar, "addCamera", addCameraComponent, this, "");
		TwAddButton(_addCompBar, "addLight", addLightComponent, this, "");
		TwAddButton(_addCompBar, "addModelRender", addModelRenderComponent, this, "");
		TwAddButton(_addCompBar, "addPhysicsBody", addPhysicsBodyComponent, this, "");
		TwAddButton(_addCompBar, "addRobotRender", addRobotRenderComponent, this, "");
		TwAddButton(_addCompBar, "addSphereCol", addSphereColComponent, this, "");
		TwAddButton(_addCompBar, "addTransform", addTransformComponent, this, "");
		TwAddButton(_addCompBar, "addCustom", addCustomComponent, this, "");
		TwAddButton(_addCompBar, "addTerrainCol", addTerrainColComponent, this, "");
		
		

		// Now link all component vars
		std::list<CompData>::iterator compData;
		int i = 0; // int to record place in iteration loop
		for(compData = goData->components.begin(); compData != goData->components.end(); ++compData)
		{
			std::string id = std::to_string(i); // convert i to a string

			// Now give each element the name "name + id" so it will be unique, but give it the label of just it's name
			switch(compData->getComp()->getType())
			{
			case ComponentType::TRANSFORM:
				TwAddVarRW(_myBar, &(id+"tx")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(0), "group=Transform label=tx");
				TwAddVarRW(_myBar, &(id+"ty")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(1), "group=Transform label=ty");
				TwAddVarRW(_myBar, &(id+"tz")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(2), "group=Transform label=tz");
				TwAddVarRW(_myBar, &(id+"rx")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(3), "group=Transform label=rx");
				TwAddVarRW(_myBar, &(id+"ry")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(4), "group=Transform label=ry");
				TwAddVarRW(_myBar, &(id+"rz")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(5), "group=Transform label=rz");
				TwAddVarRW(_myBar, &(id+"sx")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(6), "group=Transform label=sx");
				TwAddVarRW(_myBar, &(id+"sy")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(7), "group=Transform label=sy");
				TwAddVarRW(_myBar, &(id+"sz")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(8), "group=Transform label=sz");

				break;
			
			case ComponentType::CAMERA:
				TwAddVarRO(_myBar, &(id+"placeholder")[0], TW_TYPE_STDSTRING, &_noAttribs, "group=Camera label=Camera");

				break;

				
			case ComponentType::BOX_COL:
				TwAddVarRW(_myBar, &(id+"X extent")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(0), "group=Box_Col label=X_extent");
				TwAddVarRW(_myBar, &(id+"Y extent")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(1), "group=Box_Col label=Y_extent");
				TwAddVarRW(_myBar, &(id+"Z extent")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(2), "group=Box_Col label=Z_extent");
				TwAddVarRW(_myBar, &(id+"X offset")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(3), "group=Box_Col label=X_offset");
				TwAddVarRW(_myBar, &(id+"Y offset")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(4), "group=Box_Col label=Y_offset");
				TwAddVarRW(_myBar, &(id+"Z offset")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(5), "group=Box_Col label=Z_offset");
				
				break;

			case ComponentType::LIGHT:
				//TwAddVarRW(_myBar, &(id+"Light Type")[0], TW_TYPE_INT16, compData->attribPtrInt(0), "group=Light label=Light_Type");
				TwAddVarRW(_myBar, &(id+"Diffuse R")[0], TW_TYPE_FLOAT,	compData->attribPtrFloat(1), "group=Light label=Diffuse_R");
				TwAddVarRW(_myBar, &(id+"Diffuse G")[0], TW_TYPE_FLOAT,	compData->attribPtrFloat(2), "group=Light label=Diffuse_G");
				TwAddVarRW(_myBar, &(id+"Diffuse B")[0], TW_TYPE_FLOAT,	compData->attribPtrFloat(3), "group=Light label=Diffuse_B");
				TwAddVarRW(_myBar, &(id+"Specular R")[0], TW_TYPE_FLOAT,compData->attribPtrFloat(4), "group=Light label=Specular_R");
				TwAddVarRW(_myBar, &(id+"Specular G")[0], TW_TYPE_FLOAT,compData->attribPtrFloat(5), "group=Light label=Specular_G");
				TwAddVarRW(_myBar, &(id+"Specular B")[0], TW_TYPE_FLOAT,compData->attribPtrFloat(6), "group=Light label=Specular_B");

				TwAddVarRW(_myBar, &(id+"Attenuation C")[0], TW_TYPE_FLOAT,compData->attribPtrFloat(7), "group=Light label=Attenuation_C");
				TwAddVarRW(_myBar, &(id+"Attenuation L")[0], TW_TYPE_FLOAT,compData->attribPtrFloat(8), "group=Light label=Attenuation_L");
				TwAddVarRW(_myBar, &(id+"Attenuation Q")[0], TW_TYPE_FLOAT,compData->attribPtrFloat(9), "group=Light label=Attenuation_Q");

				break;

			case ComponentType::MODL_REND:
				TwAddVarRW(_myBar, &(id+"Mesh File Path")[0], TW_TYPE_STDSTRING, compData->attribPtrString(0), "group=Model_Render label=Mesh_File_Path");
				TwAddVarRW(_myBar, &(id+"Shader File Path")[0], TW_TYPE_STDSTRING, compData->attribPtrString(1), "group=Model_Render label=Shader_File_Path");
				TwAddVarRW(_myBar, &(id+"Texture File Path")[0], TW_TYPE_STDSTRING, compData->attribPtrString(2), "group=Model_Render label=Texture_File_Path");
				TwAddVarRW(_myBar, &(id+"UV Tile X")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(3), "group=Model_Render label=UV_Tile_X");
				TwAddVarRW(_myBar, &(id+"UV Tile Y")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(4), "group=Model_Render label=UV_Tile_Y");
				TwAddVarRW(_myBar, &(id+"Diffuse R")[0], TW_TYPE_FLOAT,	compData->attribPtrFloat(5), "group=Model_Render label=Diffuse_R");
				TwAddVarRW(_myBar, &(id+"Diffuse G")[0], TW_TYPE_FLOAT,	compData->attribPtrFloat(6), "group=Model_Render label=Diffuse_G");
				TwAddVarRW(_myBar, &(id+"Diffuse B")[0], TW_TYPE_FLOAT,	compData->attribPtrFloat(7), "group=Model_Render label=Diffuse_B");
				TwAddVarRW(_myBar, &(id+"Specular R")[0], TW_TYPE_FLOAT,compData->attribPtrFloat(8), "group=Model_Render label=Specular_R");
				TwAddVarRW(_myBar, &(id+"Specular G")[0], TW_TYPE_FLOAT,compData->attribPtrFloat(9), "group=Model_Render label=Specular_G");
				TwAddVarRW(_myBar, &(id+"Specular B")[0], TW_TYPE_FLOAT,compData->attribPtrFloat(10), "group=Model_Render label=Specular_B");
				TwAddVarRW(_myBar, &(id+"Specular Exponent")[0], TW_TYPE_FLOAT,compData->attribPtrFloat(11), "group=Model_Render label=Specular_Ex");
				break;

			case ComponentType::PHY_BODY:
				// None for now
				
				TwAddVarRO(_myBar, &(id+"placeholder")[0], TW_TYPE_STDSTRING, &_noAttribs, "group=Physics_Body label=Physics_Body");
				
				break;

			case ComponentType::ROB_REND:
				// None for now

				TwAddVarRO(_myBar, &(id+"placeholder")[0], TW_TYPE_STDSTRING, &_noAttribs, "group=Robot_Render label=Robot_Render");

				break;

			case ComponentType::SPHERE_COL:
				TwAddVarRW(_myBar, &(id+"Radius")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(0), "group=Sphere_Col label=Radius");
				TwAddVarRW(_myBar, &(id+"X offset")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(1), "group=Sphere_Col label=X_offset");
				TwAddVarRW(_myBar, &(id+"Y offset")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(2), "group=Sphere_Col label=Y_offset");
				TwAddVarRW(_myBar, &(id+"Z offset")[0], TW_TYPE_FLOAT, compData->attribPtrFloat(3), "group=Sphere_Col label=Z_offset");

				break;

			case ComponentType::CUSTOM:
				TwAddVarRW(_myBar, &(id+"Behaviour")[0], TW_TYPE_STDSTRING, compData->attribPtrString(0), "group=Custom label=Behaviour");
				
				break;

			case ComponentType::TERRAIN_COL:
				TwAddVarRO(_myBar, &(id+"Test")[0], TW_TYPE_STDSTRING, &_noAttribs, "group=TerrainCollider label=TerrainCollider");
				
				break;
			}

			// Advance i
			i++;
		}
		

	}
	TwRefreshBar(_myBar);
}


// This will go through each object and ensure it matches init data. It's called automatically when certain events occur
void GoMenu::refreshGameObjects()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode

	SPtr_GameObject go = _objectMngr->getGameObject(_selectedObjectID);
	if(go)
	{
		go->setName(_objName);
	}

	InitTable* initTable = _objectMngr->getInitTable();
	InitTableIterator goData;
	for(goData = initTable->begin(); goData != initTable->end(); ++goData)
	{
		// Iterate through every component and init it
		for(std::list<CompData>::iterator comp = goData->second.components.begin(); comp != goData->second.components.end(); ++comp)
		{
			comp->initializeComponent();
		}
	}

}

void GoMenu::update()
{
	// Dummy function calls on button presses for testing
	
	if(Input::getKeyPressed(sf::Keyboard::N) && Input::getKeyHeld(sf::Keyboard::LControl)) createGameObject();
	

	// Left and right cycle through game objects
	if(Input::getKeyPressed(sf::Keyboard::Left)) previousGo();
	if(Input::getKeyPressed(sf::Keyboard::Right)) nextGo();
}


void GoMenu::setGamePlaying(bool playing)
{
	_gamePlaying = playing;
}



void GoMenu::nextGo()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode

	refreshGameObjects();
	InitTable* initTable = _objectMngr->getInitTable();
	if(initTable->size() == 0) return; // to stop crash if no game objects
	InitTableIterator it = initTable->find(_selectedObjectID); // iterator 
	it = ++it;
	if(it == initTable->end()) it = initTable->begin();
	setSelectedObject(it->first);
}

void GoMenu::previousGo()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode

	refreshGameObjects();
	InitTable* initTable = _objectMngr->getInitTable();
	if(initTable->size() == 0) return; // to stop crash if no game objects
	InitTableIterator it = initTable->find(_selectedObjectID); // iterator
	if(it == initTable->begin())
	{
		it = initTable->end();
	}

	it = --it;

	setSelectedObject(it->first);
}


void GoMenu::setSelectedObject(int objID)
{
	_selectedObjectID = objID;
	SPtr_GameObject go = _objectMngr->getGameObject(_selectedObjectID);
	if(go )
	{
		SPtr_Transform trans = std::static_pointer_cast<Transform>(go->getComponent(ComponentType::TRANSFORM));
		if(trans)
		{
			_editorCam->setTarget(trans->getPosition());
		}
		else
		{
			_editorCam->setTarget(glm::vec3(0, 0, 0));
		}
	}
	else
	{
		_selectedObjectID = 0;
		_editorCam->setTarget(glm::vec3(0, 0, 0));
	}
	refreshTweakBar();
}