#include "editor\editor.h"

void Editor::startup(HINSTANCE hInstance, HWND goMenuHandle, SceneManager* sceneMngr)
{
	_hInstance = hInstance;
	_menuBar.initialize(hInstance, _goMenu, sceneMngr);
	_goMenu.initialize(hInstance, goMenuHandle);
	initConsole();
}

void Editor::update()
{
	_goMenu.update();
}

bool Editor::gamePlaying()
{
	return _menuBar.gamePlaying();
}

LRESULT Editor::mainMenuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return _menuBar.processMsg(hWnd, message, wParam, lParam);
}


LRESULT Editor::gameObjectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return _goMenu.processMsg(hWnd, message, wParam, lParam);
}



void Editor::initConsole()
{
	 int hConHandle;
    long lStdHandle;
    FILE *filePtr;

	// Create console
    AllocConsole();

	// STD out to console
    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE); // get stdo handle
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT); // get file handle using stdo handle 
    filePtr = _fdopen( hConHandle, "w" ); // pointer to file handle writes to

    *stdout = *filePtr;
    setvbuf( stdout, NULL, _IONBF, 0 );

	//STD in to console
    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    filePtr = _fdopen( hConHandle, "r" );

    *stdin = *filePtr;
    setvbuf( stdin, NULL, _IONBF, 0 );

	// STD Error to console
    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    filePtr = _fdopen( hConHandle, "w" );

    *stderr = *filePtr;
    setvbuf( stderr, NULL, _IONBF, 0 );

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
    std::ios::sync_with_stdio();
}