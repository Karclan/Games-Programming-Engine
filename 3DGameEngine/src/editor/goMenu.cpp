#include "editor\goMenu.h"
#include <iostream>

void GoMenu::initialize(ObjectManager* mngr)
{
	_objectMngr = mngr;
	_selectedObjectID = 0;
}

static void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
  // Copy the content of souceString handled by the AntTweakBar library to destinationClientString handled by your application
  destinationClientString = sourceLibraryString;
}

void GoMenu::createTweakBar()
{
	_myBar = TwNewBar("Game Object");
	refreshTweakBar();
	TwCopyStdStringToClientFunc(CopyStdStringToClient); // CopyStdStringToClient implementation is given above
}



void GoMenu::refreshTweakBar()
{
	// Remove all the variables from the tweak bar
	TwRemoveAllVars(_myBar);

	// Add the GO Name
	TwAddVarRW(_myBar, "NAME", TW_TYPE_STDSTRING, &_objName, NULL);

	// Get game object name
	_objName = "No Object Selected";
	SPtr_GameObject go = _objectMngr->getGameObject(_selectedObjectID);
	if(go)
	{
		_objName = go->getName();

		InitTable* initTable = _objectMngr->getInitTable();
		GOData* goData = &initTable->find(_selectedObjectID)->second;
		
		// Now link all component vars
		std::list<CompData>::iterator compData;
		for(compData = goData->components.begin(); compData != goData->components.end(); ++compData)
		{
			switch(compData->getComp()->getType())
			{
			case ComponentType::TRANSFORM:
				TwAddVarRW(_myBar, "tx", TW_TYPE_FLOAT, compData->attribPtrFloat(0), "");
				TwAddVarRW(_myBar, "ty", TW_TYPE_FLOAT, compData->attribPtrFloat(1), "");
				TwAddVarRW(_myBar, "tz", TW_TYPE_FLOAT, compData->attribPtrFloat(2), "");
				TwAddVarRW(_myBar, "rx", TW_TYPE_FLOAT, compData->attribPtrFloat(3), "");
				TwAddVarRW(_myBar, "ry", TW_TYPE_FLOAT, compData->attribPtrFloat(4), "");
				TwAddVarRW(_myBar, "rz", TW_TYPE_FLOAT, compData->attribPtrFloat(5), "");
				TwAddVarRW(_myBar, "sx", TW_TYPE_FLOAT, compData->attribPtrFloat(6), "");
				TwAddVarRW(_myBar, "sy", TW_TYPE_FLOAT, compData->attribPtrFloat(7), "");
				TwAddVarRW(_myBar, "sz", TW_TYPE_FLOAT, compData->attribPtrFloat(8), "");


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
	_selectedObjectID = _objectMngr->createGameObject("Bobby McCormack");
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
	_selectedObjectID = it->first;
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

	_selectedObjectID = it->first;
	refreshTweakBar();
}