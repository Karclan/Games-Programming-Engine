#ifndef EDITOR_H
#define EDITOR_H

// Required for I/O of console
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>


#include <windows.h>

#include "..\resource.h"
#include "editor\menuBar.h"
#include "editor\goMenu.h"

/*! \brief Main Editor Class

	This class contains all menus used in editor mode for creating games.

*/
class Editor
{
public:
	void startup(HINSTANCE hInstance, ObjectManager* objectMngr, SceneManager* sceneMngr);
	void update();
	bool gamePlaying(); //!< Get if the game is currently in play mode rather than edit mode

	void initTweakBars() { _goMenu.initTweakBars(); }
	int processTweakBarEvents(sf::Event* e);

	// Call backs, called from main. Can't be static as need info about editor (could make editor singleton to circumvent this but works fine for now)
	LRESULT mainMenuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	

private:
	// Private member variables
	HINSTANCE _hInstance;
	MenuBar _menuBar;
	GoMenu _goMenu;

	bool _processTwEvents; //!< Used to stop Ant Tweak Bar crashing on startup. I has a problem handling mouse move events if mouse is over window on launch!
	
	// Private functions
	void initConsole(); //!< Creates a console and redirects standard in, out and error to it so we can debug.

	

};








#endif