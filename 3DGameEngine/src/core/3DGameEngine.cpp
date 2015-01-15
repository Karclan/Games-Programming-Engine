// 3DGameEngine.cpp : Defines the entry point for the application.
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define EDITOR_MODE                     // Define to run in editor mode, remove for "stand alone"


#ifdef EDITOR_MODE
// STD for I/O of console
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#endif

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

#include "resource.h"
#include "core\engine.h"

#define MAX_LOADSTRING 100

// Global Variables:

//#ifdef EDITOR_MODE
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND _hWnd;										// The Hawkwind!
//#endif

sf::RenderWindow* myRenderWindow;
Engine engine;                                  // The engine!




#ifdef EDITOR_MODE
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// Foward declaration of own functions
void RedirectIOToConsole();
#else
void InitGameMode();

#endif



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef EDITOR_MODE
	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY3DGAMEENGINE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) // -> this is the call to function below that sets up window with SFML and whatnot
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DGAMEENGINE));

	// INIT CONSOLE
	RedirectIOToConsole();

#else // Game mode window startup

	InitGameMode();

#endif

	// INIT ENGINE
	engine.startup(myRenderWindow);


	// Whilst not quit
	bool quit = false;
	while(!quit)
	{
#ifdef EDITOR_MODE
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}  
		else
#endif
		{
			//Handle SFML events here
			sf::Event e;
			while (myRenderWindow->pollEvent(e))
			{
				if (e.type == sf::Event::Closed)
				{
					quit = true;
				}

				if(e.type == sf::Event::KeyPressed)
				{
					engine.onKeyEvent(e.key.code, KeyEvent::PRESSED); 

					if(e.key.code == sf::Keyboard::Escape)
					{
						//Quit
						quit = true;
					}
				}
				if(e.type == sf::Event::KeyReleased)
				{
					engine.onKeyEvent(e.key.code, KeyEvent::RELEASED); 
				}
				if(e.type == sf::Event::Resized)
				{
					glViewport(0,0,e.size.width,e.size.height);
				}
			}
			engine.mainLoop();
		}
#ifdef EDITOR_MODE
		if(msg.message == WM_QUIT) quit = true;
#endif
	}

	engine.shutDown();

#ifdef EDITOR_MODE
	DestroyWindow(_hWnd);
#else
	myRenderWindow->close();
#endif
	
	
	delete myRenderWindow;

	// Pause for a sec so I can read console
	float end;
	std::cin >> end;

#ifdef EDITOR_MODE
	return (int) msg.wParam;
#endif
	return 0;
}



#ifdef EDITOR_MODE
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3DGAMEENGINE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY3DGAMEENGINE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	//HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!_hWnd)
	{
		return FALSE;
	}

	// TIME TO MAKE SFML LOL
	myRenderWindow = new sf::RenderWindow(_hWnd);
	myRenderWindow->setFramerateLimit(60);
	myRenderWindow->resetGLStates();
	myRenderWindow->setTitle("Best Game Ever!");


	// Init GLEW - I think it goes here 
	glewExperimental = GL_TRUE; // glew is useless and freaks out at core stuff if you don't set this to true
    GLenum res = glewInit();
    if (res != GLEW_OK) 
	{
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    }

	// Not sure if I need this or want it here exactly or if it's correct------
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	//glViewport(0, 0, (GLsizei)1280, (GLsizei)720);

	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	ShowWindow(_hWnd, nCmdShow);
	UpdateWindow(_hWnd);

	return TRUE;
}

#else
void InitGameMode()
{
	// TIME TO MAKE SFML LOL
	myRenderWindow = new sf::RenderWindow(sf::VideoMode(200, 200), "SFML works!");
	myRenderWindow->setFramerateLimit(60);
	myRenderWindow->resetGLStates();
	myRenderWindow->setTitle("Best Game Ever!");


	// Init GLEW - I think it goes here 
	glewExperimental = GL_TRUE; // glew is useless and freaks out at core stuff if you don't set this to true
    GLenum res = glewInit();
    if (res != GLEW_OK) 
	{
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    }

	// Not sure if I need this or want it here exactly or if it's correct------
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glViewport(0, 0, (GLsizei)1280, (GLsizei)720);

	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}
#endif


#ifdef EDITOR_MODE
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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

#endif