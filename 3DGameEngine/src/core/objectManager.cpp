#include "core\objectManager.h"

ObjectManager::ObjectManager()
{
	_nextId = 0;
}


void ObjectManager::startUp(RenderSystem &rendSys, PhysicsSystem &physicsSys, BehaviourSystem &behvrSys)
{
	_rendSys = &rendSys;
	_physicsSys = &physicsSys;
	_behvrSys = &behvrSys;
	_objectFinder.setReferences(_gameObjects); // TO DO - may as well cut out middle-man and pass reference to behaviour system itself...
	_behvrSys->setObjectFinder(_objectFinder);
}

unsigned int ObjectManager::createGameObject(std::string name)
{
	// cache current id and advance next id by one
	unsigned int id = _nextId;
	_nextId += 1;

	// Add a game object and return id
	_gameObjects.emplace(id, SPtr_GameObject(new GameObject(id, name)));
	_initTable.emplace(id, GOData());
	return id;
}

bool ObjectManager::addComponent(unsigned int objectID, ComponentType::Type type)
{
	if(!addUnlinkedComponent(objectID, type)) return false;
	linkComponents(objectID);
	return true;
}


SPtr_GameObject ObjectManager::getGameObject(unsigned int objectID)
{
	// Find object by ID
	std::unordered_map<unsigned int, SPtr_GameObject>::iterator it; // iterator for searching map
	it = _gameObjects.find(objectID);
	if(it == _gameObjects.end()) 
		return SPtr_GameObject(nullptr); // Failed! Iterator == end indicates key not found
	else
		return it->second;

}


SPtr_GameObject ObjectManager::getGameObject(std::string objectName)
{
	// Find object by ID
	std::unordered_map<unsigned int, SPtr_GameObject>::iterator it; // iterator for searching map

	for(it = _gameObjects.begin(); it != _gameObjects.end(); ++it)
	{
		if(it->second->getName() == objectName) return it->second;
	}

	return SPtr_GameObject(nullptr); // Failed!
}








bool ObjectManager::addComponentsFromXML(unsigned int objectID, TiXmlElement* goElmnt)
{
	// Add all components and to Game Object and GoData
	TiXmlElement* compElmnt = goElmnt->FirstChildElement("COMP");

	for(compElmnt; compElmnt != NULL; compElmnt=compElmnt->NextSiblingElement("COMP"))
	{
		if(compElmnt->Attribute("type"))
		{
			// Add component of type, but unlinked. This will implicitly create init data objects
			int type;
			compElmnt->Attribute("type", &type);
			addUnlinkedComponent(objectID, (ComponentType::Type)type, compElmnt); // compElmnt arg allows it to be initialized to values in XML
		}
	}

	// No call link components to ensure no missing dependancies between components
	linkComponents(objectID);

	 return true;
}


void ObjectManager::destroyAll()
{
	_rendSys->clear();
	_physicsSys->clear();
	_behvrSys->clear();
	_gameObjects.clear();
	_nextId = 0;
}


// Setup any dependencies between components (e.g. a model renderer requires a reference to the transform component)
void ObjectManager::linkComponents(unsigned int goID)
{
	// For now using init table as easier to access list of components. This function could be done using _gameObjects or even actually in the GameObject class itself
	GOData* goData = &_initTable.find(goID)->second;
	SPtr_GameObject gameObject = getGameObject(goID);

	// Iterate through every component check if it requires any dependencies
	for(std::list<CompData>::iterator comp = goData->components.begin(); comp != goData->components.end(); ++comp)
	{
		// New dependency what not
		BITMASK dependencies = comp->getComp()->getDepFlags();
		BITMASK optDependencies = comp->getComp()->getOptionalDepFlags();
		
		// If has dependencies
		if(dependencies != 0)
		{
			// For each bit in the bitmask
			for(int i = 0; i < BITMASK_SIZE; ++i)
			{
				if((dependencies & 1<<i) != 0) // if bit 'i' is set
				{
					SPtr_Component desiredComponent = gameObject->getComponent((ComponentType::Type)i);
					if(!desiredComponent)
					{
						addComponent(goID, (ComponentType::Type)i);
						return; // coz addComponent will link again we can just return after this
					}
					else
					{
						comp->getComp()->linkDependency(desiredComponent);
					}
				}
			}
		}


		// If has optional dependencies
		if(optDependencies != 0)
		{
			// For each bit in the bitmask
			for(int i = 0; i < BITMASK_SIZE; ++i)
			{
				if((optDependencies & 1<<i) != 0) // if bit 'i' is set
				{
					SPtr_Component desiredComponent = gameObject->getComponent((ComponentType::Type)i);
					if(desiredComponent) comp->getComp()->linkDependency(desiredComponent);
				}
			}
		}
	}
}





//####################################################################################
//~~~~~~~~~~~~~~ FUNCTION TO EDIT WHEN MAKING NEW TYPES OF COMPONENT! ~~~~~~~~~~~~~~~
//####################################################################################


// Add a component to a game object and subsystem and generate init data for it
bool ObjectManager::addUnlinkedComponent(unsigned int objectID, ComponentType::Type type) { return addUnlinkedComponent(objectID, type, nullptr); }
bool ObjectManager::addUnlinkedComponent(unsigned int objectID, ComponentType::Type type, TiXmlElement* compElmnt)
{
	SPtr_GameObject gameObject = getGameObject(objectID);
	if(!gameObject) return false;
	SPtr_Component newComponent;


	// !-WHEN MAKING NEW COMPONENTS : TO DO - Add creation of component to this switch statement-!
	// Here is where you set creation of component based on enum
	switch(type)
	{
	case ComponentType::TRANSFORM:	newComponent.reset(new Transform());		break;
	case ComponentType::MODL_REND:	newComponent.reset(new ModelRenderer());	break;
	case ComponentType::PARTICLE_REND: newComponent.reset(new ParticleRenderer()); break;
	case ComponentType::CAMERA:		newComponent.reset(new Camera());			break;
	case ComponentType::ROB_REND:	newComponent.reset(new RobotRenderer());	break;
	case ComponentType::PHY_BODY:	newComponent.reset(new PhysicsBody());		break;
	case ComponentType::LIGHT:		newComponent.reset(new Light());			break;
	case ComponentType::SPHERE_COL:	newComponent.reset(new SphereCollider());	break;
	case ComponentType::BOX_COL:	newComponent.reset(new BoxCollider());		break;
	case ComponentType::CUSTOM:		newComponent.reset(new Custom(objectID));	break;
	}

	if(!newComponent) return false; // failed to create component, something went wrong!

	// Add component to game object
	if(!gameObject->addComponent(newComponent)) return false; // this will happen if, for example, can only have one of them and object already has one


	// !-WHEN MAKING NEW COMPONENTS : TO DO - Add sending your component to subsystems in this switch statement-!
	// Add to subsystems based on type
	switch(newComponent->getType())
	{
	case ComponentType::CAMERA: // camera - add to cameras in render system
		_rendSys->addCamera(std::static_pointer_cast<Camera>(newComponent));
		break;

	case ComponentType::MODL_REND: // model renderer - add to render system
		_rendSys->addRenderObject(std::static_pointer_cast<ModelRenderer>(newComponent));
		break;

	case ComponentType::ROB_REND:
		_rendSys->addRenderObject(std::static_pointer_cast<ModelRenderer>(newComponent));
		_rendSys->addAnimatedObject(std::static_pointer_cast<ModelRenderer>(newComponent));
		break;
	case ComponentType::PARTICLE_REND:
		_rendSys->addRenderObject(std::static_pointer_cast<ParticleRenderer>(newComponent));
		_rendSys->addAnimatedObject(std::static_pointer_cast<ParticleRenderer>(newComponent));
		break;
	case ComponentType::SPHERE_COL:
	case ComponentType::BOX_COL:
		_physicsSys->addCollider(std::static_pointer_cast<Collider>(newComponent));
		break;

	case ComponentType::LIGHT:
		_rendSys->addLight(std::static_pointer_cast<Light>(newComponent));
		break;

	case ComponentType::CUSTOM:
		_behvrSys->addCustom(std::static_pointer_cast<Custom>(newComponent));
		break;
	}

	// Add to init table
	CompData newData(newComponent);
	if(compElmnt != nullptr) newData.setAttribsFromXML(compElmnt);
	else newData.setAttribsToComponents();

	InitTableIterator inTab_it =_initTable.find(objectID);
	if(inTab_it == _initTable.end())
	{
		std::cout <<"ERROR: Game object entry does not exist is init table. Adding now but you should fix this, it is a bug!\n";
		_initTable.emplace(objectID, GOData());
		inTab_it =_initTable.find(objectID);
	}
	GOData* goData = &inTab_it->second;
	std::cout << goData->components.size();
	goData->components.push_back(newData);

	return true;
}








