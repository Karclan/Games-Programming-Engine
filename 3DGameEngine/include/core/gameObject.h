#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <vector>
#include <memory>

#include "core\component.h"
#include "rendering\camera.h"
#include "rendering\modelRenderer.h"

// Forward Declarations
class Behaviour;
typedef std::shared_ptr<Behaviour> SPtr_Behaviour;

/*! \brief Game Object

	Ultimately just a collection of components plus an ID and name to identify it.

*/
class GameObject
{
public:
	GameObject(int id, std::string name);
	~GameObject();

	SPtr_Component addComponent(SPtr_Component component); //!< Adds component then returns it. If failed, the return pointer will be null
	SPtr_Component getComponent(ComponentType::Type type); //!< Returns first component of 'type' it finds, null_ptr if can't find one
	bool checkForComponent(ComponentType::Type type);
	int getId();
	std::string getName();
	void setName(std::string name);


private:
	int _id; //!< Unique identifier for game object
	std::string _name; //!< Not guaranteed to be unique but can be easier to identify object if ID is unknown
	std::vector<SPtr_Component> _components; //!< Vector storing all the components attached to this object

};

//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<GameObject> SPtr_GameObject;

//#include "behaviour\behaviour.h"

#endif