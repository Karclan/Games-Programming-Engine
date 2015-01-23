#include "core\initTable.h"

//-----------------------------
//       COMP DATA
//-----------------------------
CompData::CompData(SPtr_Component comp)
{
	_comp = comp;

	switch(comp->getType())
	{
	case ComponentType::TRANSFORM: _priority = 0; break; // transform highest priority
	default: _priority = 100;
	}
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


void CompData::setAttribsFromXML(TiXmlElement* compElmnt)
{
	if(!compElmnt->Attribute("type")) return; // no type specified, bad tiny xml element
	int type = to_int(compElmnt, "type");

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