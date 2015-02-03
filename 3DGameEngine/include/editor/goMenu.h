#ifndef GO_MENU_H
#define GO_MENU_H

#include <windows.h>
#include <vector>
#include <antTweakBar\AntTweakBar.h>

#include "..\resource.h"

#include "core\input.h"
#include "core\initTable.h"
#include "core\objectManager.h"


const int ID_STATIC = 1000; // id of the static text thing created in func

const int ID_NEWGO_BUTTON = 1001;
const int ID_GO_LIST = 1002; //!< The id of the list with all current GOs in it
const int ID_GO_NAME = 1003;


/*! \brief Game Object Menu

	This menu handles all creation and editing of game objects
*/
class GoMenu
{
public:
	void initialize(ObjectManager* mngr);
	void update();
	void setGamePlaying(bool playing); 

	
	void initTweakBars();

	// Commands
	void newGameObject(); // create new game object


private:
	// Private member variables
	bool _gamePlaying; //!< If play mode or edito mode, set from MenuBar class
	
	TwBar* _myBar; //!< My first ant tweak bar!
	float _myFloat; // My first ant tweak bar variable

	ObjectManager* _objectMngr; //!< Pointer to the object manager

	// My test interface functiona!! ^_^
	int _selectedObjectID;

	void createGameObject();
	void createComponent();
	void deleteGameObject();
	void deleteComponent();

	bool mousePressed; //temp var for testing
	int _fixedTime;
};



#endif