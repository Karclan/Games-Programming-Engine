// 3DGameEngine.cpp : Defines the entry point for the application.
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


// STD for I/O of console
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
#include <tchar.h> // this is the only one that will cause it to crash if not included! Kept the otehrs in case issues later on...



#include <glew\GL\glew.h>
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>

#define MAX_LOADSTRING 100

HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND _hWnd;										// The Hawkwind!




// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// Foward declaration of own functions
void RedirectIOToConsole();


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	// Define a class for our main window
	WNDCLASS WindowClass;
	WindowClass.style         = 0;
	WindowClass.lpfnWndProc   = WndProc;
	WindowClass.cbClsExtra    = 0;
	WindowClass.cbWndExtra    = 0;
	WindowClass.hInstance     = hInstance;
	WindowClass.hIcon         = NULL;
	WindowClass.hCursor       = 0;
	WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
	WindowClass.lpszMenuName  = NULL;
	WindowClass.lpszClassName = L"SFML App";
	RegisterClass(&WindowClass);

	// Let's create the main window
	HWND Window = CreateWindow(L"SFML App", L"SFML Win32", WS_SYSMENU | WS_VISIBLE, 0, 0, 1280, 720, NULL, NULL, hInstance, NULL);

	// Let's create two SFML views
	DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
	HWND  View1 = CreateWindow(L"STATIC", NULL, Style, 0,  0, 200, 720, Window, NULL, hInstance, NULL);
	HWND  View2 = CreateWindow(L"STATIC", NULL, Style, 200, 0, 1080, 720, Window, NULL, hInstance, NULL);


	// INIT CONSOLE
	RedirectIOToConsole();



	sf::RenderWindow SFMLView1(View1);
	sf::RenderWindow SFMLView2(View2);


	// babby's main loop
	MSG Message;
	Message.message = ~WM_QUIT;
	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			// If a message was waiting in the message queue, process it
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			// SFML rendering code goes here
			// Clear views
			SFMLView1.clear(sf::Color::Blue);
			SFMLView2.clear(sf::Color::Red);

			// Draw sprite 1 on view 1
			//SFMLView1.draw(Sprite1);

			// Draw sprite 2 on view 2
			//SFMLView2.Draw(Sprite2);

			// Display each view on screen
			SFMLView1.display();
			SFMLView2.display();
		}
	}

	// Destroy the main window
	DestroyWindow(Window);

	// Don't forget to unregister the window class
	UnregisterClass(L"SFML App", hInstance);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
    {
        // Quit when we close the main window
        case WM_CLOSE :
        {
            PostQuitMessage(0);
            return 0;
        }
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}



// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}














// TO GET DEBUG COSNOLE
void RedirectIOToConsole()
{
    int hConHandle;
    long lStdHandle;
    FILE *fp;

	// Create console
    AllocConsole();

	// Send STDOUT to the console
    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE); // get stdo handle
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT); // get file handle using stdo handle 
    fp = _fdopen( hConHandle, "w" ); // pointer to file handle writes to

    *stdout = *fp;

    setvbuf( stdout, NULL, _IONBF, 0 );

// redirect unbuffered STDIN to the console

    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen( hConHandle, "r" );
    *stdin = *fp;
    setvbuf( stdin, NULL, _IONBF, 0 );

// redirect unbuffered STDERR to the console
    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen( hConHandle, "w" );

    *stderr = *fp;

    setvbuf( stderr, NULL, _IONBF, 0 );

// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
// point to console as well
    std::ios::sync_with_stdio();
}