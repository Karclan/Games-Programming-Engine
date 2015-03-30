#ifndef GUI_H
#define GUI_H

#include <vector>
#include "FTInterface.h"

class GUI
{
public:
	GUI();
	~GUI();

	void render();


private:

	enum Messages {Title, Lives, Score, Shield, Play, Loading, Paused};
	std::string StringMessage[7];

};

#endif