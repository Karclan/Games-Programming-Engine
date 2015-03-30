#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "behaviour\behaviour.h"
#include "physics\physicsBody.h"
#include "rendering\GUI.h"
#include "custom\playerController.h"
#include "behaviour\custom.h"
#include "core\input.h"

/*! \brief Custom Enemy Class

	Handles enemy bullet logic

*/

class TitleScreen : public Behaviour
{
public:
	TitleScreen(){};
	~TitleScreen(){};

protected:
	void initialize();
	void onActive();
	void update(float t); //!< Update function;

private:


};







#endif