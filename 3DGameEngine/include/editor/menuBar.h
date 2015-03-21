#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <windows.h>

#include "..\resource.h"
#include "editor\goMenu.h"
#include "core\engine.h"

/*! \brief The menu bar at the top of the editor

	Deals with commands such as saving and loading, stopping and playing etc
*/
class MenuBar
{
public:
	void initialize(HINSTANCE hInstance, GoMenu &goMenu, Engine* engine);
	LRESULT processMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //!< Call back passed from main through editor to this class

	bool gamePlaying(); //!< Get if the game is currently in play mode rather than edit mode

private:
	// Private callbacks. These must be static (can't have member functions as callback functions)
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void playStop(); //!< Start game playing if stopped and stop and reset game if currently playing

	// Private member variables
	HINSTANCE _hInstance;
	GoMenu* _goMenu; //!< Pointer to go menu as some commands from main menu may alter things in go menu

	Engine* _engine; //!< Needed to tell it to init game when play is pressed
	bool _gamePlaying; //!< When the game is "playing" then it is being tested. When stopped, update is not called on it and the game can be edited
};



#endif