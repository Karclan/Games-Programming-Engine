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

// Get pointers to attribs so they can be linked with editor
int* CompData::attribPtrInt(int index)
{
	return &std::static_pointer_cast<AttribDatai>(_attribs[index])->data;
}

float* CompData::attribPtrFloat(int index)
{
	return &std::static_pointer_cast<AttribDataf>(_attribs[index])->data;
}

std::string* CompData::attribPtrString(int index)
{
	return &std::static_pointer_cast<AttribDatas>(_attribs[index])->data;
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
// Basically we need to know 3 things
// 1) - How to set attribs in this data class to the corresponding variable values in actual component
// 2) - How to convert attribs from XML to corresponding attribs in this data class
// 3) - How to initialize the component, in other words how to set the variables in actual component to the values stored in this data class
// These 3 things are defined in switch statements (with a case for each component type) in the 3 functions below

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

	case ComponentType::SPHERE_COL:
		{
			SPtr_SphereCol sphereCol = std::static_pointer_cast<SphereCollider>(_comp);
			addAttribf(sphereCol->getRadius()); // radius
			addAttribf(sphereCol->getOffset().x); // offset x
			addAttribf(sphereCol->getOffset().y); // offset y
			addAttribf(sphereCol->getOffset().z); // offset z
			break;
		}

	case ComponentType::BOX_COL:
		{
			SPtr_BoxCol boxCollider = std::static_pointer_cast<BoxCollider>(_comp);
			addAttribf(boxCollider->getExtents().x); // extents x
			addAttribf(boxCollider->getExtents().y); // extents y
			addAttribf(boxCollider->getExtents().z); // extents z
			addAttribf(boxCollider->getOffset().x); // offset x
			addAttribf(boxCollider->getOffset().y); // offset y
			addAttribf(boxCollider->getOffset().z); // offset z
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

	case ComponentType::SPHERE_COL:
		addAttribf(to_float(compElmnt, "radius"));
		addAttribf(to_float(compElmnt, "ox")); // offset x
		addAttribf(to_float(compElmnt, "oy")); // offset y
		addAttribf(to_float(compElmnt, "oz")); // offset z
		break;

	case ComponentType::BOX_COL:
		addAttribf(to_float(compElmnt, "ex")); // extents x
		addAttribf(to_float(compElmnt, "ey")); // extents y
		addAttribf(to_float(compElmnt, "ez")); // extents z
		addAttribf(to_float(compElmnt, "ox")); // offset x
		addAttribf(to_float(compElmnt, "oy")); // offset y
		addAttribf(to_float(compElmnt, "oz")); // offset z
		break;


	}
}


// Initialize component's values to values stored in comp data
void CompData::initializeComponent()
{
	switch(_comp->getType())
	{
	case ComponentType::TRANSFORM:
		{
			float tx = getFloatAttrib(0);
			float ty = getFloatAttrib(1);
			float tz = getFloatAttrib(2);

			float rx = getFloatAttrib(3);
			float ry = getFloatAttrib(4);
			float rz = getFloatAttrib(5);

			float sx = getFloatAttrib(6);
			float sy = getFloatAttrib(7);
			float sz = getFloatAttrib(8);

			SPtr_Transform transform = std::static_pointer_cast<Transform>(getComp());
			transform->setPosition(glm::vec3(tx, ty, tz));
			transform->setEulerAngles(glm::vec3(rx, ry, rz));
			transform->setScale(glm::vec3(sx, sy, sz));
		}
		break;

	case ComponentType::MODL_REND:
		{
			SPtr_ModelRend model = std::static_pointer_cast<ModelRenderer>(getComp());
			
			// Figure out if the mesh is primitive or if it needs to be loaded in
			bool isPrimitive = (bool)getIntAttrib(0);

			if(isPrimitive)
			{
				int shape = getIntAttrib(1);
				model->setMesh(Assets::getPrimitiveMesh((PrimitiveShapes::Type)shape)); // set mesh
			}
			else
			{
				if(getStringAttrib(1) != "")
				{
					// TO DO...
					// Load model from file once mesh loader implemented
					model->setMesh(Assets::getMesh(getStringAttrib(1)));
				}
			}

			// Set Material
			if(getStringAttrib(3) != "") // 3 = texture path, so if not "" then it has a texture
			{
				glm::vec2 tile = glm::vec2(getFloatAttrib(4), getFloatAttrib(5)); // get tile values as vec2
				model->setMaterial(Assets::getShader(getStringAttrib(2)), Assets::getTexture(getStringAttrib(3)), tile); // set material with shader and texture
			}
			else if(getStringAttrib(2) != "") // 2 = shader path, so if not "" then it has a shader
			{
				model->setMaterial(Assets::getShader(getStringAttrib(2))); // set material with just shader
			}
		}
		break;

	case ComponentType::CAMERA:
		{
		
		}
		break;

	case ComponentType::ROB_REND:
		{
			std::shared_ptr<RobotRenderer> robot = std::static_pointer_cast<RobotRenderer>(getComp());
			robot->reset();
		}
		break;

	case ComponentType::PHY_BODY:
		{
			
		}
		break;

	case ComponentType::LIGHT:
		{
			
		}
		break;

	case ComponentType::SPHERE_COL:
		{
			SPtr_SphereCol sphereCol = std::static_pointer_cast<SphereCollider>(_comp);
			sphereCol->setRadius(getFloatAttrib(0));
			sphereCol->setOffset(glm::vec3(getFloatAttrib(1), getFloatAttrib(2), getFloatAttrib(3)));
		}
		break;

	case ComponentType::BOX_COL:
		{
			SPtr_BoxCol boxCol = std::static_pointer_cast<BoxCollider>(_comp);
			boxCol->setExtents(glm::vec3(getFloatAttrib(0), getFloatAttrib(1), getFloatAttrib(2)));
			boxCol->setOffset(glm::vec3(getFloatAttrib(3), getFloatAttrib(4), getFloatAttrib(5)));
		}
		break;
	}
}