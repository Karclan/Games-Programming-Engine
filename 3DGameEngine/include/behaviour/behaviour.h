#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "core\gameObject.h"
#include "behaviour\objectMngrInterface.h"
#include "physics\physicsBody.h"

//! Event types
enum EventType
{
	UPDATE, FIXED_UPDATE, LATE_UPDATE
};


/*! \brief Base class for all behaviours

	Behaviour is the base class for custom classes. They are wrapped
	up in the custom component so they can be passed around easier with
	less circular reference issues and then "unpacked" by the behaviour
	system

*/

class Behaviour
{
public:
	friend class BehaviourSystem; //!< This allows only the behaviour system to call base update
	Behaviour();
	~Behaviour();
	

	void linkToObject(SPtr_GameObject gameObject);
	void linkToObjectMngrInt(ObjectMngrInterface &objMngrInt);
	void reset(); //!< Recalls initialize without having to go through the inital process of adding to init list etc
	
protected:
	void addEventListener(EventType type); //!< Request certain events. Some events, like collisions, may be sent regardless (if a physics component is attached)
	virtual void initialize() = 0; //!< Children override. Called at start of first update
	virtual void onActive(){} //!< Children override. Called whenever GO set to active
	virtual void update(float t){}; //!< Children overide with unique logic.
	virtual void fixedUpdate(float t){} //!< Children overide with unique logic.
	virtual void lateUpdate(float t){} //!< Children overide with unique logic.
	SPtr_Component getComponent(ComponentType::Type type);
	int getGameObjectId();

	// Interface functions allow access to object finder
	SPtr_GameObject findGameObject(unsigned int objectID); //!< Returns an object by unique ID number
	SPtr_GameObject findGameObject(std::string objectName); //!< Searches for a game object by name and returns first one with that name or null if not found
	void addNewGameObject(SPtr_GameObject newObject); //!< Add a dynamically created Game Object to the system.
	void getCollisions(SPtr_PhysBody myPhysBody, std::vector<SPtr_GameObject> &collidingObjs); //!< Given a physics body and empty vector will populate vector with all collding objects this frame
	bool isActive();
	void setActive(bool active);
	void loadScene(std::string scene);

private:
	SPtr_GameObject _gameObject;
	ObjectMngrInterface* _objMngrInt;
	bool _initialized;
	std::vector<EventType> _requestedEvents;

	bool baseInitialize(); //!< Mainly just checks "validity" - e.g. linked to a game object. Then calls initialize if ok
	void baseUpdate(float t); //!< Called by engine, calls update() amongst other things
	bool _active;

};

//! Define shared pointer to behaviour for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<Behaviour> SPtr_Behaviour;





#endif