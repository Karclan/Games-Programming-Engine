#include "custom\endScreen.h"

void EndScreen::initialize()
{
	addEventListener(EventType::UPDATE);
}


void EndScreen::onActive()
{

}


void EndScreen::update(float t)
{
	if(Input::getKeyPressed(sf::Keyboard::Return))
	{
		loadScene("gameStart.XML");
	}
}



