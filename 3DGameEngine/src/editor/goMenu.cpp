#include "editor\goMenu.h"
#include <iostream>

void GoMenu::initialize(HINSTANCE hInstance, HWND handle, ObjectManager* mngr)
{
	_hInstance = hInstance;
	_menuHandle = handle;
	_objectMngr = mngr;

	ShowWindow(_menuHandle, SW_SHOWNORMAL);
	UpdateWindow(_menuHandle);
}

LRESULT GoMenu::processMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_CREATE:
		createMenuItems(hWnd);
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam); // the low word of wparam is the ID of the command
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_NEWGO_BUTTON:
			newGameObject();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}



void GoMenu::show()
{
	ShowWindow(_menuHandle, SW_SHOWNORMAL);
	BringWindowToTop(_menuHandle);
	SetFocus(_menuHandle);
}

void GoMenu::update()
{
	if(Input::getKeyPressed(sf::Keyboard::C)) createGameObject();
	if(Input::getKeyPressed(sf::Keyboard::D)) deleteGameObject();
	if(Input::getKeyPressed(sf::Keyboard::A)) createComponent();
	if(Input::getKeyPressed(sf::Keyboard::U)) deleteComponent();
}

void GoMenu::setGamePlaying(bool playing)
{
	_gamePlaying = playing;
}






HWND GoMenu::myCreateStatic(HWND hwnd, int id, LPCWSTR s, int x, int y, int width, int height)
// Creates a static text box.
// hwnd is the window within which the box appears,
// id is a user-provided ID number,
// x,y = position on screen,
// width, height = width and height of the box.
{  
	HWND h;

	h = CreateWindowEx(WS_EX_STATICEDGE, L"STATIC", 
		s,
		WS_CHILDWINDOW | WS_VISIBLE | SS_LEFT | SS_NOPREFIX,
		x,y,width,height,
		hwnd, 
		(HMENU) id, 
		GetModuleHandle(NULL), NULL);

	if (h) // successful creation
	{  HFONT hfDefault;
		hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(h, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
	}
	else
		MessageBox(hwnd, L"MyCreateStatic could not create static text.", 
					L"Error", MB_OK | MB_ICONERROR);

	return h;
}



void GoMenu::createMenuItems(HWND parentHandle)
{
	// Create new GO button
	HWND hWndButton=CreateWindowEx(NULL, L"BUTTON", L"New GO", WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON, 10, 2, 80, 30, parentHandle, (HMENU)ID_NEWGO_BUTTON, GetModuleHandle(NULL), NULL);

	// Create GO list
	_goListHandle = CreateWindowEx(NULL, L"COMBOBOX", L"GOs", WS_TABSTOP|WS_VISIBLE|WS_CHILD | CBS_DROPDOWNLIST , 2, 35, 180, 720, parentHandle, (HMENU)ID_GO_LIST, GetModuleHandle(NULL), NULL);

	// Text box displaying GO name
	_goNameHandle = CreateWindowEx(NULL, L"EDIT", L"Name", WS_TABSTOP|WS_VISIBLE|WS_CHILD , 2, 70, 180, 20, parentHandle, (HMENU)ID_GO_NAME, GetModuleHandle(NULL), NULL);

}

void GoMenu::newGameObject()
{
	std::string name = "New Game Object";
	std::wstring wname = stringToWString(name);
	LPCWSTR name_p = &wname[0];

	int id = SendMessage( _goListHandle, CB_ADDSTRING, 0, (LPARAM)name_p);
	//int id = SendMessage( _goListHandle, CB_ADDSTRING, 0, (LPARAM)L"New Game Object");
	if(id != CB_ERR )
	{
		GoData newObj;
		newObj.listId = id;
		_goDatas.push_back(newObj);
		SendMessage(_goListHandle, CB_SETCURSEL, id, 0);
	}
	std::cout << "New Game Object Created!\n";
}


std::wstring GoMenu::stringToWString(std::string string)
{
	// std::string -> std::wstring
	std::wstring ws;
	ws.assign(string.begin(), string.end());
	return ws;

	// std::wstring -> std::string
	//std::wstring ws(L"wstring");
	//std::string s;
	//s.assign(ws.begin(), ws.end());
}




void GoMenu::createGameObject()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode
	_selectedObjectID = _objectMngr->createGameObject("Bobby McCormack");
}
void GoMenu::createComponent()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode
	std::cout << "New Component Created!\n";
}
void GoMenu::deleteGameObject()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode
	std::cout << "Deleted Game Object!\n";
}
void GoMenu::deleteComponent()
{
	if(_gamePlaying) return; // Cannot alter objects when in play mode
	std::cout << "Deleted Component!\n";
}