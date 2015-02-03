#ifndef INIT_TABLE_H
#define INIT_TABLE_H

#include <list>
#include <vector>
#include <unordered_map>
#include <string>

#include "tinyXML\tinystr.h"
#include "tinyXML\tinyxml.h"

#include "core\assets.h"
#include "core\component.h"
#include "behaviour\behaviour.h"
#include "core\transform.h"
#include "rendering\modelRenderer.h"
#include "rendering\primitiveShapes.h"
#include "physics\physicsBody.h"


/*! \brief Data object for attributes
*/
struct AttribData{};
struct AttribDatai:AttribData { int data; }; //!< Int attrib data
struct AttribDataf:AttribData { float data; }; //!< Float attrib data
struct AttribDatas:AttribData { std::string data; }; //!< String attrib data

typedef std::shared_ptr<AttribData> SPtr_AttribData;





/*! \brief Data object for components
*/
class CompData
{
public:
	CompData(SPtr_Component comp);
	~CompData();
	void addAttribi(int data); //!< Add an int attribute
	void addAttribf(float data); //!< Add a float attribute
	void addAttribs(std::string data); //!< Add a string attribute
	void setAttribsToComponents(); //!< Set all of the attibs to whatever values the actual component currently has
	void setAttribsFromXML(TiXmlElement* compElmnt); //!< Set all of the attibs based on a tiny xml element
	

	int getIntAttrib(int index); //!< Get an int attribute by index
	float getFloatAttrib(int index); //!< Get a float attribute by index
	std::string getStringAttrib(int index); //!< Get a string attribute by index
	int attribCount(); //!< Total Number of attributes
	SPtr_Component getComp(); //!< Get pointer to actual component object

private:
	SPtr_Component _comp; //!< Pointer to the actual component object
	std::vector<SPtr_AttribData> _attribs; //!< Attributes used as args in initialization. Bools and enums etc are stored as ints

	// Helper functions for getting tinyxml data more easily
	int to_int(TiXmlElement* elmnt, std::string attribute);
	float to_float(TiXmlElement* elmnt, std::string attribute);

};






/*! \brief Data object for game objects
*/
struct GOData
{
	//std::string name;
	std::list<CompData> components;
	std::vector<SPtr_Behaviour> behaviours;
};







/*! \brief Info on how to initialize a scene

	The init table is a representation of a scene XML file in local memory.
	It can be used to create and initalize a new scene or it can be used to
	reset a scene back to its initial state without having to reload from XML.
	The latter use is especially needed for editor mode, when constant testing
	from the init state is needed. The init table is implemented as a vector of
	Game Object datas, each with a list of components and behavaiours and their innit
	values (used a typedef to make it easy to change to full class if more functiaonality needed)

*/
typedef std::unordered_map<unsigned int, GOData> InitTable;
typedef std::unordered_map<unsigned int, GOData>::iterator InitTableIterator;


#endif