#include "core\input.h"
#include <iostream> // for debugging

Input* Input::startUp(sf::RenderWindow* window)
{
	Input* inst = Input::get();
	inst->_window = window;
	return inst;
}

Input* Input::get()
{
	static Input input;
	return &input;
}

// Static functions designed to be called by anything in game
bool Input::getKeyPressed(int key)
{
	Input* ins = Input::get();
	for(EvntIt it = ins->_events.begin(); it != ins->_events.end(); ++it)
	{
		if(it->_key == key && it->_state == KeyEvent::PRESSED) return true;
	}
	return false;
}

bool Input::getKeyHeld(int key)
{
	Input* ins = Input::get();
	for(EvntIt it = ins->_events.begin(); it != ins->_events.end(); ++it)
	{
		if(it->_key == key && it->_state == KeyEvent::HELD) return true;
		else if(it->_key == key && it->_state == KeyEvent::PRESSED) return true; // pressed also counts as held
	}
	return false;
}

bool Input::getKeyReleased(int key)
{
	Input* ins = Input::get();
	for(EvntIt it = ins->_events.begin(); it != ins->_events.end(); ++it)
	{
		if(it->_key == key && it->_state == KeyEvent::RELEASED) return true;
	}
	return false;
}

bool Input::getMouseHeld(int button)
{
	return sf::Mouse::isButtonPressed(sf::Mouse::Button(button));
}

sf::Vector2i Input::getMousePos()
{
	return get()->_currentMousePos;
}

sf::Vector2f Input::getMouseDelta()
{
	return get()->_mouseDelta;
}

float Input::getScrollWheelDelta()
{
	return get()->_scrollWheelDelta;
}


// CALLED BY ENGINE
void Input::processEvent(sf::Event e)
{
	switch(e.type)
	{
	case sf::Event::KeyPressed:			keyDown(KeyEvent(KeyEvent::State::PRESSED, e.key.code));	 break;
	case sf::Event::KeyReleased:		keyUp(e.key.code);											 break;
	case sf::Event::MouseWheelMoved:	scrollWheelMoved(e.mouseWheel.delta);						 break;
	}
}

void Input::update(float t)
{
	// Mouse stuff - these probably should be "early" update functions rather than late updates. So consider putting in another function later
	_currentMousePos = sf::Mouse::getPosition(*_window);
	_mouseDelta = sf::Vector2f(_currentMousePos.x - _lastMousePos.x, _currentMousePos.y - _lastMousePos.y) * t;
	_lastMousePos = _currentMousePos;
	_scrollWheelDelta = _scrollWheelMove * t;
}


void Input::reset()
{
	EvntIt it = _events.begin();
	while (it != _events.end())
	{
		switch(it->_state)
		{
		case KeyEvent::PRESSED:
			it->_state = KeyEvent::HELD;
		case KeyEvent::HELD:
			++it; // fall through, advance it if state is pressed or held
			break;

		case KeyEvent::RELEASED:
			it = _events.erase(it);
			break;

		default:
			++it;
			break;
		}
	}

	// reset scroll wheel to zero
	_scrollWheelMove = 0;
}






// non-static private functions
void Input::keyDown(KeyEvent keyEvent)
{
	_events.push_back(keyEvent);
}

void Input::keyUp(int key)
{
	for(EvntIt it = _events.begin(); it != _events.end(); ++it)
	{
		if(it->_key == key) it->_state = KeyEvent::RELEASED;
	}
}

void Input::scrollWheelMoved(float amount)
{
	_scrollWheelMove = amount;
}