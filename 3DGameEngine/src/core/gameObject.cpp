#include "core\gameObject.h"
#include <iostream>

GameObject::GameObject(unsigned int id, std::string name)
{
	_id = id;
	_name = name;
	_createdDyn = false;
	_lifeCycleState = ComponentState::ACTIVE;
}

GameObject::~GameObject()
{
	std::cout << "Game Object DeSTORYed lol\n";
	removeFromSystem();
}


void GameObject::removeFromSystem()
{
	for(unsigned int i = 0; i < _components.size(); ++i)
	{
		_components[i]->destroy(); // sets component state to destroy. Other systems should pick this up and stop referencing it, ultimately deleting it via shared pointer
	}
	_components.clear();
}

void GameObject::setActive(bool active)
{
	switch(active)
	{
	case false: _lifeCycleState = ComponentState::INACTIVE; break;
	case true: _lifeCycleState = ComponentState::ACTIVE; break;
	}

	// And for all components
	for(int i = 0; i < _components.size(); ++i)
	{
		_components[i]->setActive(active);
	}
}


// The reason we return component ptr (null if fail) rather than bool for fail is slightly greater flexibility
// For example, if we ensure all systems do a null check when passed a component ptr then it doesn't matter
// if object creator forgets to check for success.
SPtr_Component GameObject::addComponent(SPtr_Component component)
{
	if(component) // if not null
	{
		// If allowed more than one per game object, add to component list and return success
		if(!component->isOnePerObject())
		{
			_components.push_back(component);
			return component;
		}
		
		// If reached this code then only one of this component is allowed per object!
		if(!checkForComponent(component->getType())) // if doesn't already have one of these then add
		{
			_components.push_back(component);
			return component;
		}
	}

	// Return fail
	component.reset(); // sets it to nullptr
	return component;
}



SPtr_Component GameObject::getComponent(ComponentType::Type type)
{
	for(unsigned int i = 0; i < _components.size(); ++i)
	{
		if(_components[i]->getType() == type) return _components[i];
	}
	return SPtr_Component();
}



bool GameObject::checkForComponent(ComponentType::Type type)
{
	for(unsigned int i = 0; i < _components.size(); ++i)
	{
		if(_components[i]->getType() == type) return true;
	}
	return false;
}

unsigned int GameObject::getId()
{
	return _id;
}

std::string GameObject::getName()
{
	return _name;
}

void GameObject::setName(std::string name)
{
	_name = name;
}