#include "core\initTable.h"

//-----------------------------
//       COMP DATA
//-----------------------------
CompData::CompData(SPtr_Component comp)
{
	_comp = comp;
}

CompData::~CompData()
{
	_attribs.clear();
}


void CompData::addAttribi(int data)
{
	std::shared_ptr<AttribDatai> newInt(new AttribDatai());
	newInt->data = data;
	_attribs.push_back(newInt);
}

void CompData::addAttribf(float data)
{
	std::shared_ptr<AttribDataf> newFloat(new AttribDataf());
	newFloat->data = data;
	_attribs.push_back(newFloat);
}

void CompData::addAttribs(std::string data)
{
	std::shared_ptr<AttribDatas> newString(new AttribDatas());
	newString->data = data;
	_attribs.push_back(newString);
}

int CompData::getIntAttrib(int index)
{ 
	return std::static_pointer_cast<AttribDatai>(_attribs[index])->data;
}

float CompData::getFloatAttrib(int index) 
{
	return std::static_pointer_cast<AttribDataf>(_attribs[index])->data;
}

std::string CompData::getStringAttrib(int index) 
{ 
	return std::static_pointer_cast<AttribDatas>(_attribs[index])->data; 
}

int CompData::attribCount() 
{
	return _attribs.size();
}

SPtr_Component CompData::getComp() 
{ 
	return _comp;
}


// Helpers
int CompData::to_int(TiXmlElement* elmnt, std::string attribute)
{
	int val;
	elmnt->Attribute(attribute, &val);
	return val;
}

float CompData::to_float(TiXmlElement* elmnt, std::string attribute)
{
	double val;
	elmnt->Attribute(attribute, &val);
	return val;
}




//####################################################################################
//~~~~~~~~~~~~~~ FUNCTIONS TO EDIT WHEN MAKING NEW TYPES OF COMPONENT! ~~~~~~~~~~~~~~~
//####################################################################################

// The default attribs - please ensure correct number and type for this component's initialization
void CompData::setAttribsToComponents()
{
	switch(_comp->getType())
	{
	case ComponentType::TRANSFORM:
		{
			SPtr_Transform trans = std::static_pointer_cast<Transform>(_comp);
			addAttribf(trans->getPosition().x); // tx
			addAttribf(trans->getPosition().y); // ty
			addAttribf(trans->getPosition().z); // tz
			addAttribf(trans->getEulerAngles().x); // rx
			addAttribf(trans->getEulerAngles().y); // ry
			addAttribf(trans->getEulerAngles().z); // rz
			addAttribf(trans->getScale().x); // sx;
			addAttribf(trans->getScale().y); // sy;
			addAttribf(trans->getScale().z); // sz
			break;
		}

	case ComponentType::CAMERA:
		{
			SPtr_Camera camera = std::static_pointer_cast<Camera>(_comp);
			break;
		}




	case ComponentType::MODL_REND:
		{
			SPtr_ModelRend modelRend = std::static_pointer_cast<ModelRenderer>(_comp);
			Mesh* mesh = modelRend->getMesh();
			int prim = -1; // if primitive mesh will be something other than -1
			
			if(mesh != nullptr)
			{
				prim = modelRend->getMesh()->getPrimID();
				if(prim != -1)
				{
					addAttribi((int)true); // attrib 0 is if primitive mesh or not
					addAttribi(prim); // attrib 1 is prim ID if prim
				}
				else
				{
					addAttribi((int)false); // attrib 0 is if primitive mesh or not
					addAttribs(modelRend->getMesh()->getFilePath()); // attrib 1 is filepath of mesh if not prim
				}
			}
			else
			{
				addAttribi((int)false); // attrib 0 is if primitive mesh or not
				addAttribs(""); // attrib 1 is prim ID if prim
			}
			
			Material* mat = modelRend->getMaterial();
			addAttribs(mat->getShaderFilePath()); // attrib 2 is shader
			addAttribs(mat->getTextureFilePath()); // attrib 3 is texture file path as string
			addAttribf(mat->getUvTile().x); // attrib 4 is tile u
			addAttribf(mat->getUvTile().y); // attrib 5 is tile v
			break;
		}





	case ComponentType::ROB_REND:
		break;

	case ComponentType::PHY_BODY:
		{
			SPtr_PhysBody physBody = std::static_pointer_cast<PhysicsBody>(_comp);
			break;
		}


	}
}
//----------



// Read attribs from xml - please ensure correct number, type and order for this component's initialization
void CompData::setAttribsFromXML(TiXmlElement* compElmnt)
{
	if(!compElmnt->Attribute("type")) // no type specified, bad tiny xml element
	{
		setAttribsToComponents();
		return; 
	}
	int type = to_int(compElmnt, "type");

	if(type != _comp->getType()) // does match this type of component
	{
		setAttribsToComponents();
		return; 
	}

	// Read data from XML into attribs based on component type
	switch(type)
	{
	case ComponentType::TRANSFORM:
		addAttribf(to_float(compElmnt, "tx"));
		addAttribf(to_float(compElmnt, "ty"));
		addAttribf(to_float(compElmnt, "tz"));
		addAttribf(to_float(compElmnt, "rx"));
		addAttribf(to_float(compElmnt, "ry"));
		addAttribf(to_float(compElmnt, "rz"));
		addAttribf(to_float(compElmnt, "sx"));
		addAttribf(to_float(compElmnt, "sy"));
		addAttribf(to_float(compElmnt, "sz"));
		break;

	case ComponentType::CAMERA:
		break;





	case ComponentType::MODL_REND:
		{
		int isPrimitive = to_int(compElmnt, "primitive"); // attrib 0 is if primitive mesh or not
		addAttribi(isPrimitive);
		if(isPrimitive) addAttribi(to_int(compElmnt, "mesh")); // attrib 1 is prim ID or...
		else addAttribs(compElmnt->Attribute("mesh")); // ...filepath of mesh if not prim
		addAttribs(compElmnt->Attribute("shader")); // attrib 2 is shader
		addAttribs(compElmnt->Attribute("texture")); // attrib 3 is texture file path as string
		addAttribf(to_float(compElmnt, "tileU")); // attrib 4 is tile u
		addAttribf(to_float(compElmnt,"tileV")); // attrib 5 is tile v
		break;
		}





	case ComponentType::ROB_REND:
		break;

	case ComponentType::PHY_BODY:
		break;


	}
}