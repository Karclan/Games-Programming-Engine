#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <list>
#include <SFML\Graphics.hpp>

//! Struct used for storing current state of pressed keys
struct KeyEvent
{
	enum State { PRESSED, HELD, RELEASED };
	State _state;
	int _key;
	int _mods;

	KeyEvent(State state, int key)
	{
		_state = state;
		_key = key;
	}
};


/*! \brief Input class (Singleton)

	Singleton class allowing interface through static functions to get input information.
	Currently there is a slight bug where key released events are lost if the window loses
	focus whilst a key is held. 

*/
class Input
{
public:
	// Startup function should only be called once, by engine, to init class and get a pointer to the instance
	static Input* startUp(sf::RenderWindow* window);

	static bool getKeyPressed(int key);
	static bool getKeyHeld(int key);
	static bool getKeyReleased(int key);
	static bool getMouseHeld(int button);
	static sf::Vector2i getMousePos();
	static sf::Vector2f getMouseDelta();
	static float getScrollWheelDelta();
	

	// non-static public functions
	void processEvent(sf::Event e); //!< Process event to see if input event
	void update(float t); //!< Called at start of game loop, straight after events polled
	void reset(); //!< Called from engine at the end of the game loop to set key states etc

private:
	Input(){};
	~Input(){};

	// private functions
	static Input* get(); // private so only this class can access the instance, others must interface through static functions
	void keyDown(KeyEvent keyEvent);
	void keyUp(int key);
	void scrollWheelMoved(float amount);

	// variables
	Input* _this;
	std::list<KeyEvent> _events;
	sf::RenderWindow* _window;
	sf::Vector2i _currentMousePos; //!< Current position of mouse relative to game window
	sf::Vector2i _lastMousePos; //!< Mouse pos previous frame
	sf::Vector2f _mouseDelta; //!< Amount mouse has moved multiplied by delta time to make frame rate independent
	float _scrollWheelMove; //!< Amount mouse scroll wheel has moved this frame
	float _scrollWheelDelta; //!< Scroll Wheel Move multiplied by delta time to make frame rate independent

	typedef std::list<KeyEvent>::iterator EvntIt;
};

#endif