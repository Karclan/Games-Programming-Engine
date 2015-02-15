#ifndef GO_MENU_H
#define GO_MENU_H

#include <windows.h>
#include <vector>
#include <antTweakBar\AntTweakBar.h>

#include "..\resource.h"

#include "core\input.h"
#include "core\initTable.h"
#include "core\objectManager.h"
#include "core\sceneManager.h"
#include "editor\editorCamera.h"


/*! \brief Game Object Menu

	This menu handles all creation and editing of game objects
*/
class GoMenu
{
public:
	void initialize(ObjectManager* objMngr, SceneManager* sceneMngr, EditorCamera* editorCam);
	void update();
	void setGamePlaying(bool playing); 

	
	void createTweakBar();

	// Commands
	void newGameObject(); // create new game object
	void refreshGameObjects(); //!< Refreshes all game objects to match init data


	void addComponent(ComponentType::Type type);



private:
	// Private member variables
	bool _gamePlaying; //!< If play mode or edito mode, set from MenuBar class

	bool testBool;
	std::string filePath;
	
	TwBar* _myBar; //!< My first ant tweak bar!
	TwBar* _addCompBar;	//!< Used for adding components to a Game Object

	ObjectManager* _objectMngr; //!< Pointer to the object manager
	SceneManager* _sceneMngr; //!< We need this to save to XML
	EditorCamera* _editorCam; //!< Pointer to editor cam so can set target
	
	
	// Vars in Menu
	InitTableIterator _currentGodataIter;
	std::string _objName; //! Name of GO currently selected



	// My test interface functiona!! ^_^
	int _selectedObjectID;

	void setSelectedObject(int objID);

	void refreshTweakBar(); //!< Refreshes tweak bar to match currently selected object

	void createGameObject();
	void createComponent();
	void deleteGameObject();
	void deleteComponent();

	

	void nextGo();
	void previousGo();
};



#endif