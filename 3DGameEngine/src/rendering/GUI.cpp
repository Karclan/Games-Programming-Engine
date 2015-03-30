#include "rendering/GUI.h"

GUI::GUI()
{
	//initialise the messages
	StringMessage[Lives] = "Life:  ";
	StringMessage[Score] = "Score: ";
}
GUI::~GUI(){}

void GUI::render()
{
	FTInterface::renderText(StringMessage[Lives], 300, 5, 1, glm::vec3(1,1,1));
	FTInterface::renderText(StringMessage[Score], 600, 15,1, glm::vec3(1,1,1));
}