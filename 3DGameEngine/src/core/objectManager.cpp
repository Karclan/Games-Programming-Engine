#include "core\objectManager.h"

ObjectManager::ObjectManager()
{
	_nextId = 0;
}


ObjectManager* ObjectManager::startUp(RenderSystem &rendSys, PhysicsSystem &physicsSys)
{
	ObjectManager* inst = ObjectManager::get();
	inst->_rendSys = &rendSys;
	inst->_physicsSys = &physicsSys;
	return inst;
}

ObjectManager* ObjectManager::get()
{
	static ObjectManager objectMngr;
	return &objectMngr;
}




SPtr_GameObject ObjectManager::getGameObject(unsigned int objectID)
{
	// Find object by ID
	std::unordered_map<unsigned int, SPtr_GameObject>::iterator it; // iterator for searching map
	it = get()->_gameObjects.find(objectID);
	if(it == get()->_gameObjects.end()) 
		return SPtr_GameObject(nullptr); // Failed! Iterator == end indicates key not found
	else
		return it->second;

}


SPtr_GameObject ObjectManager::getGameObject(std::string objectName)
{
	// Find object by ID
	std::unordered_map<unsigned int, SPtr_GameObject>::iterator it; // iterator for searching map

	for(it = get()->_gameObjects.begin(); it != get()->_gameObjects.end(); ++it)
	{
		if(it->second->getName() == objectName) return it->second;
	}

	return SPtr_GameObject(nullptr); // Failed!
}


unsigned int ObjectManager::createGameObject(std::string name)
{
	// cache current id and advance next id by one
	unsigned int id = _nextId;
	_nextId += 1;

	// Add a game object and return id
	_gameObjects.emplace(id, SPtr_GameObject(new GameObject(id, name)));
	return id;
}

bool ObjectManager::addComponent(unsigned int ObjectID, SPtr_Component component)
{
	// Find object by ID
	std::unordered_map<unsigned int, SPtr_GameObject>::iterator it; // iterator for searching map
	it = _gameObjects.find(ObjectID);
	if(it == _gameObjects.end()) return false; // Failed! Iterator == end indicates key not found

	SPtr_Component addedComponent = it->second->addComponent(component);
	if(!addedComponent) return false; // this will happen if, for example, can only have one of them and object already has one

	// Add to subsystems based on type
	switch(addedComponent->getType())
	{
	case ComponentType::CAMERA: // camera - add to cameras in render system
		_rendSys->addCamera(std::static_pointer_cast<Camera>(addedComponent));
		break;

	case ComponentType::MODL_REND: // model renderer - add to render system
		_rendSys->addRenderObject(std::static_pointer_cast<ModelRenderer>(addedComponent));
		break;

	case ComponentType::ROB_REND:
		_rendSys->addRenderObject(std::static_pointer_cast<ModelRenderer>(addedComponent));
		_rendSys->addAnimatedObject(std::static_pointer_cast<ModelRenderer>(addedComponent));
		break;
	}

	return true;
}




void ObjectManager::destroyAll()
{
	_gameObjects.clear();
	_nextId = 0;
}