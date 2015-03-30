#ifndef GUI_H
#define GUI_H

#include "core\component.h"
#include "FTInterface.h"
#include <string>
#include <vector>


class GUI : public Component
{
public:
	GUI();
	~GUI(){};

	virtual ComponentType::Type getType() { return ComponentType::GUI; }
	virtual bool isOnePerObject() { return false; }
	
	void setMessage(std::string m,  int x, int y, float s, glm::vec3 c, bool b);
	void updateMessage(int messageNum, std::string s);
	void setRender(int messageNum, bool b);

	void render();
private:

	std::vector<std::string> _messages;
	std::vector<float> _scale;
	std::vector<int> _xPos;
	std::vector<int> _yPos;
	std::vector<glm::vec3> _colour;
	std::vector<bool> _render;


};
	typedef std::shared_ptr<GUI> SPtr_GUI;
#endif