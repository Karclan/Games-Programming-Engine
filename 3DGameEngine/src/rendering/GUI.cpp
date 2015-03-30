#include "rendering\GUI.h"

GUI::GUI()
{

}

void GUI::render()
{
	for(int i = 0; i < _messages.size(); ++i)
	{
		if(_render[i] == true)
		{
			FTInterface::renderText(_messages[i], _xPos[i], _yPos[i], _scale[i], _colour[i]);
		}
	}
}

void GUI::setMessage(std::string m,  int x, int y, float s, glm::vec3 c, bool b)
{
	_messages.push_back(m);
	_scale.push_back(s);
	_xPos.push_back(x);
	_yPos.push_back(y);
	_colour.push_back(c);
	_render.push_back(b);
}

void GUI::updateMessage(int messageNum, std::string s)
{
	_messages[messageNum] = s;
}

void GUI::setRender(int messageNum, bool b)
{
	_render[messageNum] = b;
}