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
	TwAddVarRW(_myBar, "NAME", TW_TYPE_STDSTRING, &_objName, NULL);
	TwCopyStdStringToClientFunc(CopyStdStringToClient); // CopyStdStringToClient implementation is given above
}



void GoMenu::refreshTweakBar()
{
	// Get game object name
	_objName = "No Object Selected";
	SPtr_GameObject go = _objectMngr->getGameObject(_selectedObjectID);
	if(go)
	{
		_objName = go->getName();
	}
	//TwAddVarRW(_myBar, "NAME", TW_TYPE_STDSTRING, &_objName, NULL);

}

void GoMenu::setParamsFromTweakBar()
{
	SPtr_GameObject go = _objectMngr->getGameObject(_selectedObjectID);
	if(go)
	{
		go->setName(_objName);
	}

}

void GoMenu::update()
{
	if(Input::getKeyPressed(sf::Keyboard::C)) createGameObject();
	if(Input::getKeyPressed(sf::Keyboard::D)) deleteGameObject();
	if(Input::getKeyPressed(sf::Keyboard::A)) createComponent();
	if(Input::getKeyPressed(sf::Keyboard::U)) deleteComponent();

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
	setParamsFromTweakBar();
	InitTable* initTable = _objectMngr->getInitTable();
	InitTableIterator it = initTable->find(_selectedObjectID); // iterator 
	it = ++it;
	if(it == initTable->end()) it = initTable->begin();
	_selectedObjectID = it->first;
	refreshTweakBar();
}

void GoMenu::previousGo()
{
	setParamsFromTweakBar();
	InitTable* initTable = _objectMngr->getInitTable();
	InitTableIterator it = initTable->find(_selectedObjectID); // iterator 
	it = --it;
	if(it == initTable->end())
	{
		it = initTable->end();
		it = --it;
	}
	_selectedObjectID = it->first;
	refreshTweakBar();
}