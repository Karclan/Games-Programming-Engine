#include "core\objectManager.h"

ObjectManager::ObjectManager()
{
	// Note ID 0 is reserved for checks to see if ID has been set or not.
	_nextId = 1;
	_nextDynamicId = 1;
}


void ObjectManager::startUp(RenderSystem &rendSys, PhysicsSystem &physicsSys, BehaviourSystem &behvrSys)
{
	_rendSys = &rendSys;
	_physicsSys = &physicsSys;
	_behvrSys = &behvrSys;
	_objMngrInt.setReferences(_gameObjects, _dynInitObjs);
	_behvrSys->setObjMngrInt(_objMngrInt);
}

void ObjectManager::initGame()
{
	_nextDynamicId = _nextId; // reset dynamic id indices
	
	std::unordered_map<unsigned int, SPtr_GameObject>::iterator it;
	std::vector<int> dynObjs;
	for(it = _gameObjects.begin(); it != _gameObjects.end(); ++it)
	{
		if(it->second.get()->getCreatedDyn())
		{
			dynObjs.push_back(it->first); // get ID of every dynamically created object
		}
	}

	for(int i = 0; i < dynObjs.size(); ++i)
	{
		it = _gameObjects.find(dynObjs[i]); // find each dynamically created object	

		// Remove all components from subsystems
		const std::vector<SPtr_Component>* comps = it->second->getComponents();
		for(int j = 0; j < comps->size(); ++j)
		{
			removeComponentFromSubsystems(comps->at(j));
		}

		// Remove Game Object from system
		_gameObjects.erase(it);
	}
}



// Called every frame from Engine. If there are dynamically created objects, they are added to the system
void ObjectManager::initDynamicObjects()
{
	if(_dynInitObjs.size() == 0) return;
	std::set<SPtr_GameObject>::iterator it;
	for(it = _dynInitObjs.begin(); it != _dynInitObjs.end(); ++it)
	{
		// Set ID to correct ID and put in map
		it->get()->setId(_nextDynamicId);
		_gameObjects.emplace(_nextDynamicId, *it);
		
		
		// Add components to subsystems
		const std::vector<SPtr_Component>* components = it->get()->getComponents();
		for(int i = 0; i < components->size(); ++i)
		{
			addComponentToSubsystems(components->at(i), _nextDynamicId);
		}

		// Link components
		linkComponents(_nextDynamicId);

		// Inc dynamic id
		_nextDynamicId++;
	}

	// Clear init objects
	_dynInitObjs.clear();
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
	_nextId = 1;
}


// Setup any dependencies between components (e.g. a model renderer requires a reference to the transform component)
void ObjectManager::linkComponents(unsigned int goID)
{
	// For now using init table as easier to access list of components. This function could be done using _gameObjects or even actually in the GameObject class itself
	SPtr_GameObject gameObject = getGameObject(goID);
	const std::vector<SPtr_Component>* components = gameObject->getComponents();
	std::vector<SPtr_Component>::const_iterator compIt = components->begin();

	// Iterate through every component check if it requires any dependencies
	for(compIt; compIt != components->end(); ++compIt)
	{
		// New dependency what not
		BITMASK dependencies = compIt->get()->getDepFlags();
		BITMASK optDependencies = compIt->get()->getOptionalDepFlags();
		
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
						compIt->get()->linkDependency(desiredComponent);
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
					if(desiredComponent) compIt->get()->linkDependency(desiredComponent);
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
	case ComponentType::TRANSFORM:		newComponent.reset(new Transform());		break;
	case ComponentType::MODL_REND:		newComponent.reset(new ModelRenderer());	break;
	case ComponentType::PARTICLE_REND:  newComponent.reset(new ParticleRenderer()); break;
	case ComponentType::CAMERA:			newComponent.reset(new Camera());			break;
	case ComponentType::ROB_REND:		newComponent.reset(new RobotRenderer());	break;
	case ComponentType::PHY_BODY:		newComponent.reset(new PhysicsBody());		break;
	case ComponentType::LIGHT:			newComponent.reset(new Light());			break;
	case ComponentType::SPHERE_COL:		newComponent.reset(new SphereCollider());	break;
	case ComponentType::BOX_COL:		newComponent.reset(new BoxCollider());		break;
	case ComponentType::CUSTOM:			newComponent.reset(new Custom(objectID));	break;
	case ComponentType::TERRAIN_COL:	newComponent.reset(new TerrainCollider());	break;
	case ComponentType::ANIMATION:		newComponent.reset(new Animator());			break;
	}

	if(!newComponent) return false; // failed to create component, something went wrong!

	// Add component to game object
	if(!gameObject->addComponent(newComponent)) return false; // this will happen if, for example, can only have one of them and object already has one


	addComponentToSubsystems(newComponent, objectID);

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


// Note - split this up from addComponent function so can be used for dynamic objetcs as well
void ObjectManager::addComponentToSubsystems(SPtr_Component newComponent, unsigned int goID)
{
	newComponent->setGoID(goID);

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
		_rendSys->addRenderObject(std::static_pointer_cast<RobotRenderer>(newComponent));
		break;
	case ComponentType::PARTICLE_REND:
		//_rendSys->addRenderObject(std::static_pointer_cast<ParticleRenderer>(newComponent));
		_rendSys->addParticleSystem(std::static_pointer_cast<ParticleRenderer>(newComponent));
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

	case ComponentType::TERRAIN_COL:
		_physicsSys->addTerrainCollider(std::static_pointer_cast<TerrainCollider>(newComponent));
		break;

	case ComponentType::ANIMATION:
		_rendSys->addAnimator(std::static_pointer_cast<Animator>(newComponent));
		break;
	}

}



// Remove object from subsystem
void ObjectManager::removeComponentFromSubsystems(SPtr_Component component)
{
	
	// !-WHEN MAKING NEW COMPONENTS : TO DO - Ensure your component can be removed from subsystems you added it to!
	// Add to subsystems based on type
	switch(component->getType())
	{
	case ComponentType::CAMERA: // camera - add to cameras in render system
		_rendSys->removeCamera(std::static_pointer_cast<Camera>(component));
		break;

	case ComponentType::MODL_REND: // model renderer - add to render system
		_rendSys->removeRenderObject(std::static_pointer_cast<Renderer>(component));
		break;

	case ComponentType::SPHERE_COL:
	case ComponentType::BOX_COL:
		_physicsSys->removeCollider(std::static_pointer_cast<Collider>(component));
		break;

	case ComponentType::LIGHT:
		_rendSys->removeLight(std::static_pointer_cast<Light>(component));
		break;

	case ComponentType::CUSTOM:
		_behvrSys->removeCustom(std::static_pointer_cast<Custom>(component));
		break;

	case ComponentType::TERRAIN_COL:
		_physicsSys->removeTerrainCollider(std::static_pointer_cast<TerrainCollider>(component));
		break;

	case ComponentType::ANIMATION:
		_rendSys->removeAnimator(std::static_pointer_cast<Animator>(component));
		break;
	}
	

}







