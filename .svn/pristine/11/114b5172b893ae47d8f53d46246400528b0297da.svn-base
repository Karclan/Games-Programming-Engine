// 3DGameEngine.cpp : Defines the entry point for the application.
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define EDITOR_MODE                     // Define to run in editor mode, remove for "stand alone"

#include <windows.h> // main windows header file
#include <tchar.h> // needed for generic text mappings

#include <glew\GL\glew.h>
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>

#include "resource.h"
#include "core\engine.h"

#ifdef EDITOR_MODE //--------------------DECLARATIONS FOR EDITOR ONLY-----------------
#include "editor\editor.h"
#include "editor\editorCamera.h"
#define MAX_LOADSTRING 100

TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
TCHAR szGOMenuClass[MAX_LOADSTRING];			// the game object menu class name

Editor editor; // instance of the editor class
EditorCamera editorCamera; // special camera for rendering in "edit" mode

// Forward declarations of functions such as callbacks
void regWindowsClasses(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM); // main meu commands
LRESULT CALLBACK GameObjectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // game object menu commands
#endif              //--------------------END EDITOR DECLARATIONS-----------------

// Main engine - needed in both editor and game mode
sf::Clock updateClock; // clock for update times
Engine engine;


// MAIN!!!------------
// _tWinMain is the entry point for the program
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);



#ifdef EDITOR_MODE /*------ LAUNCH IN EDITOR MODE ------- */
	HACCEL hAccelTable; // not sure what this is. Something to do with hardware acceleration? Default code in new win api project...
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DGAMEENGINE));
	
	// Register our windows classes (function below main)
	regWindowsClasses(hInstance);

	// Create the main window
	HWND windowHandle = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!windowHandle) return FALSE;	
	sf::RenderWindow* mainWindow = engine.startEditorMode(windowHandle); // Link window with SFML RenderWindow in engine
	
	// Create inspector window
	HWND goMenuHandle = CreateWindow(szGOMenuClass, L"Game Objects", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 200, 720, NULL, NULL, hInstance, NULL);
	if(!goMenuHandle) return false;

	// Startup editor and engine
	editor.startup(hInstance, goMenuHandle, engine.getSceneManager());
	engine.startup();
	BringWindowToTop(goMenuHandle);
	editorCamera.init();

	// To capture and dispatch windows message
	MSG msg;

#else            /*------ LAUNCH IN GAME MODE ------- */
	sf::RenderWindow* mainWindow = engine.startGameMode(); // create game mode SFML window
	engine.startup(); // start engine
#endif



	// START MAIN LOOP
	bool quit = false; // Will be set to true if either winAPI or SFML quit event / msg
	while(!quit)
	{

#ifdef EDITOR_MODE //---------MSG HANDLING FOR EDITOR MODULES----------//
		// If a message was waiting in the message queue, process it
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Perhaps we're ignoring hardware msgs so SFML can process them??
			if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) // .. unless it's something to do with hardware accel?
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if(msg.message == WM_QUIT) quit = true;
		}  
		else
#endif //---------------------END EDITOR MODULE MSG HANDLING---------//

		// SFML Event polling
		{
			sf::Event e;
			while (mainWindow->pollEvent(e))
			{
				// Quit
				if (e.type == sf::Event::Closed) quit = true;

				// Resize view
				else if(e.type == sf::Event::Resized) glViewport(0, 0, e.size.width, e.size.height);

				// Engine events
				else engine.processEvent(e);
			}

			// Get delta time and reset clock
			float deltaTime = updateClock.getElapsedTime().asSeconds();
			updateClock.restart();


#ifndef EDITOR_MODE //---------------MAIN LOOP FOR GAME MODE
			// Call main loop function in engine
			engine.updateInput(deltaTime);
			engine.update(deltaTime);
			engine.render();
			engine.resetInput();
#else//-----------------------------MAIN LOOP FOR EDITOR MODE
			// Call main loop function in engine
			engine.updateInput(deltaTime);
			editor.update();
			
			// Update differently based on if in play mode or stop mode
			if(editor.gamePlaying())
			{
				engine.update(deltaTime);
				engine.render();
			}
			else
			{
				editorCamera.update(deltaTime);
				engine.render(editorCamera.getCamera());
			}

			engine.resetInput();
#endif
		}
	}// END MAIN LOOP

	engine.shutDown();

#ifdef EDITOR_MODE
	DestroyWindow(windowHandle);
	DestroyWindow(goMenuHandle);

	// Don't forget to unregister the window class
	UnregisterClass(szWindowClass, hInstance);
	UnregisterClass(szGOMenuClass, hInstance);
#endif
	
	// Pause for a sec so I can read console
	float end;
	std::cin >> end;

#ifdef EDITOR_MODE
	return (int) msg.wParam;
#endif
	return 0;
}



#ifdef EDITOR_MODE
void regWindowsClasses(HINSTANCE hInstance)
{
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY3DGAMEENGINE, szWindowClass, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMEOBJECTMENU, szGOMenuClass, MAX_LOADSTRING);
	
	// Define a class for our main window
	WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style         = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc   = WndProc;
	windowClass.cbClsExtra    = 0;
	windowClass.cbWndExtra    = 0;
	windowClass.hInstance     = hInstance;
	windowClass.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3DGAMEENGINE));
	windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND); // = (HBRUSH)(COLOR_WINDOW+1);
	windowClass.lpszMenuName  = MAKEINTRESOURCE(IDC_MY3DGAMEENGINE);
	windowClass.lpszClassName = szWindowClass;
	windowClass.hIconSm = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&windowClass);

	//--------------------------------------------------
	// Define a class for game object menu
	
	WNDCLASS goMenuClass;
	goMenuClass.style         = CS_NOCLOSE;
	goMenuClass.lpfnWndProc   = GameObjectProc;
	goMenuClass.cbClsExtra    = 0;
	goMenuClass.cbWndExtra    = 0;
	goMenuClass.hInstance     = hInstance;
	goMenuClass.hIcon         = NULL;
	goMenuClass.hCursor       = 0;
	goMenuClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
	goMenuClass.lpszMenuName  = NULL;
	goMenuClass.lpszClassName = szGOMenuClass;
	RegisterClass(&goMenuClass);
	
	//--------------------------------------------------------------
}

// Call back functions. Forward params to editor for processing
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return editor.mainMenuProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK GameObjectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return editor.gameObjectProc(hWnd, message, wParam, lParam);
}


#endif