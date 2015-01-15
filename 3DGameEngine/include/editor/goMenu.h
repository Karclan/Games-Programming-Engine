#ifndef GO_MENU_H
#define GO_MENU_H

#include <windows.h>
#include <vector>

#include "..\resource.h"

#include "core\sceneManager.h"


const int ID_STATIC = 1000; // id of the static text thing created in func

const int ID_NEWGO_BUTTON = 1001;
const int ID_GO_LIST = 1002; //!< The id of the list with all current GOs in it
const int ID_GO_NAME = 1003;

/*! \brief Temporary placeholder - real-time generating of game objects not yet implemented
*/
struct GoData
{
	int listId;
};

/*! \brief Game Object Menu

	This menu handles all creation and editing of game objects
*/
class GoMenu
{
public:
	void initialize(HINSTANCE hInstance, HWND handle);
	LRESULT processMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //!< Call back passed from main through editor to this class
	void show(); //!<Brings menu to front
	void update();


	// Test functions
	HWND myCreateStatic(HWND hwnd, int id, LPCWSTR s, int x, int y, int width, int height);


	// Create menu items
	void createMenuItems(HWND parentHandle);

	// Commands
	void newGameObject(); // create new game object


private:
	// Private member variables
	HINSTANCE _hInstance; // might not need this but may as well leave for now
	HWND _menuHandle; //!< The handle for the go menu
	HWND _goListHandle; //!< Handle for the drop down menu containing all GOs
	HWND _goNameHandle; //!< Handle to the text field for entering game object's name

	std::vector<GoData> _goDatas; //!< Data about every created game object


	std::wstring stringToWString(std::string string); //!< Function for converting from std::string to wstring
};



#endif