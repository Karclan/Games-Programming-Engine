#include "core\input.h"
#include <iostream> // for debugging

Input* Input::startUp(sf::RenderWindow* window)
{
	Input* inst = Input::get();
	inst->_window = window;
	for(int i = 0; i < 4; ++i)
	{
		inst->_joysticks[i].reset();
	}
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

bool Input::isJoyStickConnected(int joystickID)
{
	return get()->_joysticks[joystickID]._active;
}

bool Input::getJoystickButtonPressed(int joyStickID, JoystickButtons::Buttons button)
{
	return  get()->_joysticks[joyStickID]._buttons[button];
}

float Input::getJoystickAxisPosition(int joyStickID, sf::Joystick::Axis axis)
{
	return get()->_joysticks[joyStickID]._axisValues[axis];
}


// CALLED BY ENGINE
void Input::processEvent(sf::Event e)
{
	switch(e.type)
	{
	case sf::Event::KeyPressed:				keyDown(KeyEvent(KeyEvent::State::PRESSED, e.key.code));										break;
	case sf::Event::KeyReleased:			keyUp(e.key.code);																				break;
	case sf::Event::MouseWheelMoved:		scrollWheelMoved(e.mouseWheel.delta);															break;
	case sf::Event::JoystickButtonPressed:	joyStickbuttonDown	(e.joystickButton.joystickId,e.joystickButton.button);						break;
	case sf::Event::JoystickButtonReleased:	joyStickbuttonUp	(e.joystickButton.joystickId,e.joystickButton.button);						break;
	case sf::Event::JoystickConnected:		joyStickConnected	(e.joystickConnect.joystickId);												break;
	case sf::Event::JoystickDisconnected:	joyStickDisconnected(e.joystickConnect.joystickId);												break;
	case sf::Event::JoystickMoved:			joyStickAxisMoved	(e.joystickMove.joystickId,e.joystickMove.axis,e.joystickMove.position);	break;
	}
}

void Input::update(float t)
{
	// Mouse stuff - these probably should be "early" update functions rather than late updates. So consider putting in another function later
	_currentMousePos = sf::Mouse::getPosition(*_window);
	_mouseDelta = sf::Vector2f(_currentMousePos.x - _lastMousePos.x, _currentMousePos.y - _lastMousePos.y) * t;
	_lastMousePos = _currentMousePos;
	_scrollWheelDelta = _scrollWheelMove * t;
	sf::Joystick::update();
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

	for(auto it: _joysticks)
	{
		it.resetValues();
	}
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

void Input::joyStickConnected(int joystickId)
{
	std::cout<<"Joystick :"<<joystickId<<" connected!\n";
	_joysticks[joystickId]._active=true;
}
void Input::joyStickDisconnected(int joystickId)
{
	std::cout<<"Joystick :"<<joystickId<<" disconnected!\n";
	_joysticks[joystickId]._active=false;
}
void Input::joyStickbuttonDown(int joystickId, int joystickButton)
{
	std::cout<<"Joystick: "<<joystickId<<" button: "<<joystickButton<<" Pressed!\n";
	_joysticks[joystickId]._buttons[joystickButton]=true;
}
void Input::joyStickbuttonUp(int joystickId, int joystickButton)
{
	std::cout<<"Joystick: "<<joystickId<<" button: "<<joystickButton<<" Released!\n";
	_joysticks[joystickId]._buttons[joystickButton]=false;
}
void Input::joyStickAxisMoved(int joystickId, sf::Joystick::Axis axis, float position)
{
	switch (axis)
	{
	case sf::Joystick::X: //Stick 1 X Axis
		//std::cout<<"Joystick: "<<joystickId<<" X Axis: "<<axis<<" value: "<<position<<"\n";
		break;
	case sf::Joystick::Y: //Stick 2 X Axis
		//std::cout<<"Joystick: "<<joystickId<<" Y Axis: "<<axis<<" value: "<<position<<"\n";
		break;
	case sf::Joystick::Z: //Trigger Axis
		//std::cout<<"Joystick: "<<joystickId<<" Z Axis: "<<axis<<" value: "<<position<<"\n";
		break;
	case sf::Joystick::R: //Stick 2 Y axis
		//std::cout<<"Joystick: "<<joystickId<<" R Axis: "<<axis<<" value: "<<position<<"\n";
		break;
	case sf::Joystick::U: //Stick 2 X axis
		//std::cout<<"Joystick: "<<joystickId<<" U Axis: "<<axis<<" value: "<<position<<"\n";
		break;
	case sf::Joystick::V:  //UNUSED AXIS//
		std::cout<<"Joystick: "<<joystickId<<" V Axis: "<<axis<<" value: "<<position<<"\n";
		break;
	case sf::Joystick::PovX: //D Pad
		//std::cout<<"Joystick: "<<joystickId<<" PovX: "<<axis<<" value: "<<position<<"\n";
		break;
	case sf::Joystick::PovY: //D Pad
		//std::cout<<"Joystick: "<<joystickId<<" PovY: "<<axis<<" value: "<<position<<"\n";
		break;
	default:
		std::cout<<"unknown axis\n";
		break;
	}
	_joysticks[joystickId]._axisValues[axis]=position;
}

