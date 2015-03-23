#include "core\engine.h"

sf::RenderWindow* Engine::startEditorMode(HWND windowHandle)
{
	_window.create(windowHandle);
	_window.setFramerateLimit(60);
	_window.resetGLStates();

	// Init GLEW
	glewExperimental = GL_TRUE; // glew is useless and freaks out at core stuff if you don't set this to true
    GLenum res = glewInit();
    if (res != GLEW_OK) fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));

	// OpenGL render settings
	glDepthMask(GL_TRUE); // ensure depth buffer is ok to read and write to
	glEnable(GL_DEPTH_TEST); // ensure depth testing on so stuff draws in right order
	glEnable(GL_CULL_FACE); // backface culling for better performance
	
	// Show main window
	ShowWindow(windowHandle, SW_SHOWNORMAL);
	UpdateWindow(windowHandle);

	return &_window;
}

sf::RenderWindow* Engine::startGameMode()
{
	// TIME TO MAKE SFML LOL
	_window.create(sf::VideoMode(1280, 720), "Game Engine");
	_window.setFramerateLimit(60);
	_window.resetGLStates();


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

	return &_window;
}



void Engine::startup()
{
	// Clear colour
	//_window = window;
    glClearColor(0.5f, 0.8f, 1.0f, 0.0f);

	//=================================================
	//=================================================
	// OpenGL is good to go!!
	// Time to initialize all core engine systems - can decompose this into separate function later if it gets messy
	_rendSys.setShadersMap(Assets::getShadersMap()); // link map of shaders to rendering system
	_objMngr.startUp(_rendSys, _physicsSys, _behvrSys); // this will need all sub systems somehow. Perhaps consider 1 function for each sub system, more code but neater
	_sceneMngr.initialize(_objMngr, _rendSys); // init and pass reference to object manager
	
	_sceneMngr.loadFromXML(DEMO_SCENE_PATH); // this loads the demo scene from XML

	FTInterface::init();

	// Setup input
	_input = Input::startUp(&_window);

	// Set screen vars
	_screenSize = sf::Vector2u(1280, 720);
	_window.setSize(_screenSize);

	// init time
	_refreshRate = 1.0f/60.0f;
	_fixedTime = 0; //fixed delta time
}


void Engine::processEvent(sf::Event e)
{
	_input->processEvent(e);
}


void Engine::updateInput(float t)
{
	_input->update(t); // update just before polling, this will set previously pressed heys to "held"
}


void Engine::update(float t)
{
	// Add time to fixed time
	_fixedTime += t;

	// Update
	_behvrSys.update(t);

	// Fixed update
	if(_fixedTime >= _refreshRate)
	{
		// fixed update here
		_behvrSys.fixedUpdate(_fixedTime);
		_physicsSys.fixedUpdate(_fixedTime);
		_fixedTime = 0;
	}

	// Animate
	_rendSys.animate(t);
}


void Engine::render()
{
	// Render
	///first pass
	//glClear(GL_DEPTH_BUFFER_BIT);
	//render everything to back buffer

	//second pass
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen
	_rendSys.render();

	_window.display(); // switch buffers
	//_window.pushGLStates();
	//_window.resetGLStates(); // think you have to reset GL states before drawing any SFML
	//draw sfml
	//_window.resetGLStates();
	//_window.popGLStates();
}


void Engine::renderEditorMode(Camera* camera)
{
	// Render
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen
	
	_rendSys.render(camera); // render everything to back buffer
	FTInterface::renderText("Editor Mode",0,0,1,glm::vec3(0.5f,0.1f,0.8f));
	_physicsSys.renderColliders(camera); // render debug colliders in physics
	TwDraw();  // draw the tweak bar(s)
	_window.display(); // switch buffers

}


void Engine::resetInput()
{
	_input->reset();
}


void Engine::shutDown()
{
	_behvrSys.clear();
	_rendSys.clear();
	_objMngr.destroyAll();
	_objMngr.clearInitTable();
	Assets::unloadAllAssets();
}


// Getters / Setters
SceneManager* Engine::getSceneManager()
{
	return &_sceneMngr;
}

ObjectManager* Engine::getObjectManager()
{
	return &_objMngr;
}

int Engine::getWidth()
{
	return _window.getSize().x;
}

int Engine::getHeight() 
{ 
	return _window.getSize().y;
}