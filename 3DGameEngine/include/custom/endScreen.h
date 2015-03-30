#ifndef END_SCREEN_H
#define END_SCREEN_H

#include "behaviour\behaviour.h"
#include "physics\physicsBody.h"
#include "rendering\GUI.h"
#include "custom\playerController.h"
#include "behaviour\custom.h"
#include "core\input.h"

/*! \brief Custom Enemy Class

	Handles enemy bullet logic

*/

class EndScreen : public Behaviour
{
public:
	EndScreen(){};
	~EndScreen(){};

protected:
	void initialize();
	void onActive();
	void update(float t); //!< Update function;

private:


};







#endif