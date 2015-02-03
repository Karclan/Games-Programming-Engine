#include "editor\goMenu.h"
#include <iostream>

void GoMenu::initialize(ObjectManager* mngr)
{
	_objectMngr = mngr;
}



void GoMenu::initTweakBars()
{
	_myBar = TwNewBar("Babby's First");
	TwAddVarRW(_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &_myFloat, NULL);
	mousePressed = false;
	_fixedTime = 0;
}



void GoMenu::update()
{
	if(Input::getKeyPressed(sf::Keyboard::C)) createGameObject();
	if(Input::getKeyPressed(sf::Keyboard::D)) deleteGameObject();
	if(Input::getKeyPressed(sf::Keyboard::A)) createComponent();
	if(Input::getKeyPressed(sf::Keyboard::U)) deleteComponent();
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