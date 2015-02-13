#include "editor\goMenu.h"
#include <iostream>

void GoMenu::initialize(ObjectManager* mngr, EditorCamera* editorCam)
{
	_objectMngr = mngr;
	_editorCam = editorCam;
	testBool = false;
}

static void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
  // Copy the content of souceString handled by the AntTweakBar library to destinationClientString handled by your application
  destinationClientString = sourceLibraryString;
}

void GoMenu::createTweakBar()
{
	_myBar = TwNewBar("Game Object");
	_addCompBar = TwNewBar("Add Component");
	refreshTweakBar();
	TwCopyStdStringToClientFunc(CopyStdStringToClient); // CopyStdStringToClient implementation is given above
	setSelectedObject(0);
}

static void TW_CALL addCameraComponent(void *clientData)
{
	//GoMenu* goMenu = static_cast<GoMenu>(clientData);
	GoMenu* goMenu = (GoMenu*)clientData;
	goMenu->testFunc();

	// gameObject.addComponent();
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

		TwAddVarRW(_addCompBar, "test", TW_TYPE_BOOL8, &testBool, "");
		TwAddButton(_addCompBar, "buttonTest", addCameraComponent, this, "");
		
		//TwAddVarRW(_addCompBar, "hasCameraComponent", TW_TYPE_BOOL8, &hasCamera, "");


		// Now link all component vars
		std::list<CompData>::iterator compData;
		for(compData = goData->components.begin(); compData != goData->components.end(); ++compData)
		{
			switch(compData->getComp()->getType())
			{
			case ComponentType::TRANSFORM:
				TwAddVarRW(_myBar, "tx", TW_TYPE_FLOAT, compData->attribPtrFloat(0), "group=Transform");
				TwAddVarRW(_myBar, "ty", TW_TYPE_FLOAT, compData->attribPtrFloat(1), "group=Transform");
				TwAddVarRW(_myBar, "tz", TW_TYPE_FLOAT, compData->attribPtrFloat(2), "group=Transform");
				TwAddVarRW(_myBar, "rx", TW_TYPE_FLOAT, compData->attribPtrFloat(3), "group=Transform");
				TwAddVarRW(_myBar, "ry", TW_TYPE_FLOAT, compData->attribPtrFloat(4), "group=Transform");
				TwAddVarRW(_myBar, "rz", TW_TYPE_FLOAT, compData->attribPtrFloat(5), "group=Transform");
				TwAddVarRW(_myBar, "sx", TW_TYPE_FLOAT, compData->attribPtrFloat(6), "group=Transform");
				TwAddVarRW(_myBar, "sy", TW_TYPE_FLOAT, compData->attribPtrFloat(7), "group=Transform");
				TwAddVarRW(_myBar, "sz", TW_TYPE_FLOAT, compData->attribPtrFloat(8), "group=Transform");

				break;
			
			case ComponentType::CAMERA:

				// None to add for now

				TwAddVarRW(_myBar, "placeholder", TW_TYPE_FLOAT, "nothing", "group=Camera");

				break;

				
			case ComponentType::BOX_COL:

				// TwAddVarRW(_myBar, "placeholder", TW_TYPE_FLOAT, "nothing", "group=Box Col");

				TwAddVarRW(_myBar, "X extent", TW_TYPE_FLOAT, compData->attribPtrFloat(0), "group=Box_Col");
				TwAddVarRW(_myBar, "Y extent", TW_TYPE_FLOAT, compData->attribPtrFloat(1), "group=Box_Col");
				TwAddVarRW(_myBar, "Z extent", TW_TYPE_FLOAT, compData->attribPtrFloat(2), "group=Box_Col");
				TwAddVarRW(_myBar, "X offset", TW_TYPE_FLOAT, compData->attribPtrFloat(3), "group=Box_Col");
				TwAddVarRW(_myBar, "Y offset", TW_TYPE_FLOAT, compData->attribPtrFloat(4), "group=Box_Col");
				TwAddVarRW(_myBar, "Z offset", TW_TYPE_FLOAT, compData->attribPtrFloat(5), "group=Box_Col");
				
				break;

			case ComponentType::LIGHT:

				TwAddVarRW(_myBar, "placeholder", TW_TYPE_FLOAT, "nothing", "group=Light");

				break;

			case ComponentType::MODL_REND:

				//TwAddVarRW(_myBar, "placeholder", TW_TYPE_FLOAT, "nothing", "group=Model Render");

				TwAddVarRW(_myBar, "Mesh File Path", TW_TYPE_STDSTRING, compData->attribPtrString(0), "group=Model_Render");
				TwAddVarRW(_myBar, "Shader File Path", TW_TYPE_STDSTRING, compData->attribPtrString(1), "group=Model_Render");
				TwAddVarRW(_myBar, "Texture File Path", TW_TYPE_STDSTRING, compData->attribPtrString(2), "group=Model_Render");
				TwAddVarRW(_myBar, "UV Tile X", TW_TYPE_FLOAT, compData->attribPtrFloat(3), "group=Model_Render");
				TwAddVarRW(_myBar, "UV Tile Y", TW_TYPE_FLOAT, compData->attribPtrFloat(4), "group=Model_Render");

				break;

			case ComponentType::PHY_BODY:

				// None for now

				TwAddVarRW(_myBar, "placeholder", TW_TYPE_FLOAT, "nothing", "group=Physics_Body");
				
				break;

			case ComponentType::ROB_REND:

				// None for now

				TwAddVarRW(_myBar, "placeholder", TW_TYPE_FLOAT, "nothing", "group=Robot_Render");

				break;

			case ComponentType::SPHERE_COL:

				// TwAddVarRW(_myBar, "placeholder", TW_TYPE_FLOAT, "nothing", "group=Sphere Col");

				TwAddVarRW(_myBar, "Radius", TW_TYPE_FLOAT, compData->attribPtrFloat(0), "group=Sphere_Col");
				TwAddVarRW(_myBar, "X offset", TW_TYPE_FLOAT, compData->attribPtrFloat(1), "group=Sphere_Col");
				TwAddVarRW(_myBar, "Y offset", TW_TYPE_FLOAT, compData->attribPtrFloat(2), "group=Sphere_Col");
				TwAddVarRW(_myBar, "Z offset", TW_TYPE_FLOAT, compData->attribPtrFloat(3), "group=Sphere_Col");

				break;
				
			}

		}
		

	}
	//TwAddVarRW(_myBar, "NAME", TW_TYPE_STDSTRING, &_objName, NULL);
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
	
	if(Input::getKeyPressed(sf::Keyboard::C)) createGameObject();
	if(Input::getKeyPressed(sf::Keyboard::D)) deleteGameObject();
	if(Input::getKeyPressed(sf::Keyboard::A)) createComponent();
	if(Input::getKeyPressed(sf::Keyboard::U)) deleteComponent();
	

	// Left and right cycle through game objects
	if(Input::getKeyPressed(sf::Keyboard::Left)) previousGo();
	if(Input::getKeyPressed(sf::Keyboard::Right)) nextGo();
}


void GoMenu::setGamePlaying(bool playing)
{
	_gamePlaying = playing;
}

void GoMenu::createGameObject()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode
	setSelectedObject(_objectMngr->createGameObject("Bobby McCormack"));
	refreshTweakBar();
}
void GoMenu::createComponent()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode
	std::cout << "New Component Created!\n";
}
void GoMenu::deleteGameObject()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode
	std::cout << "Deleted Game Object!\n";
}
void GoMenu::deleteComponent()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode
	std::cout << "Deleted Component!\n";
}




void GoMenu::nextGo()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode

	refreshGameObjects();
	InitTable* initTable = _objectMngr->getInitTable();
	InitTableIterator it = initTable->find(_selectedObjectID); // iterator 
	it = ++it;
	if(it == initTable->end()) it = initTable->begin();
	setSelectedObject(it->first);
	refreshTweakBar();
}

void GoMenu::previousGo()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode

	refreshGameObjects();
	InitTable* initTable = _objectMngr->getInitTable();
	InitTableIterator it = initTable->find(_selectedObjectID); // iterator
	if(it == initTable->begin())
	{
		it = initTable->end();
	}

	it = --it;

	setSelectedObject(it->first);
	refreshTweakBar();
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
		_editorCam->setTarget(glm::vec3(0, 0, 0));
	}

}