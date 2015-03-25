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



// Set Attribs
void CompData::setIntAttrib(int index, int value)
{
	std::shared_ptr<AttribDatai> data = std::static_pointer_cast<AttribDatai>(_attribs[index]);
	data->data = value;
}
void CompData::setFloatAttrib(int index, float value)
{
	std::shared_ptr<AttribDataf> data = std::static_pointer_cast<AttribDataf>(_attribs[index]);
	data->data = value;
}
void CompData::setStringAttrib(int index, std::string value)
{
	std::shared_ptr<AttribDatas> data = std::static_pointer_cast<AttribDatas>(_attribs[index]);
	data->data = value;
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

			// Mesh
			Mesh* mesh = modelRend->getMesh();
			if(mesh) addAttribs(modelRend->getMesh()->getFilePath()); // attrib 0 is filepath of mesh
			else addAttribs(""); // blank string if no mesh

			// Material
			Material* mat = modelRend->getMaterial();
			addAttribs(mat->getShaderFilePath()); // attrib 1 is shader
			addAttribs(mat->getTextureFilePath(Material::DIFFUSE)); // attrib 2 is texture file path as string
			addAttribs(mat->getTextureFilePath(Material::SPECULAR)); // attrib 3 is texture file path as string
			addAttribs(mat->getTextureFilePath(Material::NORMAL)); // attrib 4 is texture file path as string
			addAttribs(mat->getTextureFilePath(Material::HEIGHT)); // attrib 5 is texture file path as string
			addAttribf(mat->getUvTile().x); // attrib 6 is tile u
			addAttribf(mat->getUvTile().y); // attrib 7 is tile v		
			
			addAttribf(mat->getDiffuse().x);	//diffuse r
			addAttribf(mat->getDiffuse().y);	//diffuse g
			addAttribf(mat->getDiffuse().z);	//diffuse b
			addAttribf(mat->getSpecular().x);	//specular r
			addAttribf(mat->getSpecular().y);	//specular g
			addAttribf(mat->getSpecular().z);	//specular b
			addAttribf(mat->getSpecularExp());	//specular exponent
			break;
		}

	case ComponentType::ROB_REND:
		break;

	case ComponentType::PARTICLE_REND:
		{
			SPtr_ParticleRend particleRender = std::static_pointer_cast<ParticleRenderer>(_comp);
			addAttribi(particleRender->getPoolSize());   						//poolSize				| attrib:  0
//X//		//===BOX POSITION GENERATOR===//																	   
			addAttribi(particleRender->_boxPosGen.isActive());					//boxPosGenActive		| attrib:  1
			addAttribf(particleRender->_boxPosGen.getStartOffset().x);			//boxPosOffX			| attrib:  2
			addAttribf(particleRender->_boxPosGen.getStartOffset().y);			//boxPosOffY			| attrib:  3
			addAttribf(particleRender->_boxPosGen.getStartOffset().z);			//boxPosOffZ			| attrib:  4
			addAttribf(particleRender->_boxPosGen.getStartOffset().w);			//boxPosOffW			| attrib:  5	
//X//		//===ROUND POSITION GENERATOR===//																	   
			addAttribi(particleRender->_roundPosGen.isActive());				//roundPosGenActive	| attrib:  6
			addAttribf(particleRender->_roundPosGen.getRadius().x);				//radiusX				| attrib:  7
			addAttribf(particleRender->_roundPosGen.getRadius().y);				//radiusY				| attrib:  8
//X//		//===BASIC COLOUR GENERATOR===//
			addAttribi(particleRender->_basicColourGen.isActive());				//bColourGenActive		| attrib:  9
			addAttribf(particleRender->_basicColourGen.getMinStartColour().x);	//bMinStColX			| attrib: 10
			addAttribf(particleRender->_basicColourGen.getMinStartColour().y);	//bMinStColY			| attrib: 11
			addAttribf(particleRender->_basicColourGen.getMinStartColour().z);	//bMinStColZ			| attrib: 12
			addAttribf(particleRender->_basicColourGen.getMinStartColour().w);	//bMinStColW			| attrib: 13
			addAttribf(particleRender->_basicColourGen.getMaxStartColour().x);	//bMaxStColX			| attrib: 14
			addAttribf(particleRender->_basicColourGen.getMaxStartColour().y);	//bMaxStColY			| attrib: 15
			addAttribf(particleRender->_basicColourGen.getMaxStartColour().z);	//bMaxStColZ			| attrib: 16
			addAttribf(particleRender->_basicColourGen.getMaxStartColour().w);	//bMaxStColW			| attrib: 17
			addAttribf(particleRender->_basicColourGen.getMinEndColour().x);	//bMinEndColX			| attrib: 18
			addAttribf(particleRender->_basicColourGen.getMinEndColour().y);	//bMinEndColY			| attrib: 19
			addAttribf(particleRender->_basicColourGen.getMinEndColour().z);	//bMinEndColZ			| attrib: 20
			addAttribf(particleRender->_basicColourGen.getMinEndColour().w);	//bMinEndColW			| attrib: 21
			addAttribf(particleRender->_basicColourGen.getMaxEndColour().x);	//bMaxEndColX			| attrib: 22
			addAttribf(particleRender->_basicColourGen.getMaxEndColour().y);	//bMaxEndColY			| attrib: 23
			addAttribf(particleRender->_basicColourGen.getMaxEndColour().z);	//bMaxEndColZ			| attrib: 24
			addAttribf(particleRender->_basicColourGen.getMaxEndColour().w);	//bMaxEndColW			| attrib: 25
//X//		//===BASIC VELOCITY GENERATOR===//
			addAttribi(particleRender->_basicVelGen.isActive());				//basicVelGenActive		| attrib: 26
			addAttribf(particleRender->_basicVelGen.getMinStartVel().x);		//bMinVelX				| attrib: 27
			addAttribf(particleRender->_basicVelGen.getMinStartVel().w);		//bMinVelY				| attrib: 38
			addAttribf(particleRender->_basicVelGen.getMinStartVel().z);		//bMinVelZ				| attrib: 29
			addAttribf(particleRender->_basicVelGen.getMinStartVel().w);		//bMinVelW				| attrib: 30
			addAttribf(particleRender->_basicVelGen.getMaxStartVel().x);		//bMaxVelX				| attrib: 31
			addAttribf(particleRender->_basicVelGen.getMaxStartVel().w);		//bMaxVelY				| attrib: 32
			addAttribf(particleRender->_basicVelGen.getMaxStartVel().z);		//bMaxVelZ				| attrib: 33
			addAttribf(particleRender->_basicVelGen.getMaxStartVel().w);		//bMaxVelW				| attrib: 34
//X//		//===SPHERE VELOCITY GENERATOR===//
			addAttribi(particleRender->_sphereVelGen.isActive());				//bSphereGenActive		| attrib: 35
			addAttribf(particleRender->_sphereVelGen.getMinVelocity());			//bSMinVel				| attrib: 36
			addAttribf(particleRender->_sphereVelGen.getMaxVelocity());			//bSMaxVel				| attrib: 37
//X//		//===BASIC TIME GENERATOR===//
			addAttribi(particleRender->_basicTimeGen.isActive());				//bTimeGenActive		| attrib: 38
			addAttribf(particleRender->_basicTimeGen.getMinTime());				//minTime				| attrib: 39
			addAttribf(particleRender->_basicTimeGen.getMaxTime());				//maxTime				| attrib: 40
			//===EULER UPDATER===//
			addAttribi(particleRender->_eulerUpdater.isActive());				//eulUpdActive			| attrib: 41
			addAttribf(particleRender->_eulerUpdater.getGlobalAcc().x);			//globalAccX			| attrib: 42
			addAttribf(particleRender->_eulerUpdater.getGlobalAcc().y);			//globalAccY			| attrib: 43
			addAttribf(particleRender->_eulerUpdater.getGlobalAcc().z);			//globalAccZ			| attrib: 44
			addAttribf(particleRender->_eulerUpdater.getGlobalAcc().w);			//globalAccW			| attrib: 45
//X//		//==ATTRACTOR UPDATER==//
			addAttribi(particleRender->_attractorUpdater.isActive());			//attUpdActive			| attrib: 46
			addAttribi(particleRender->_attractorUpdater.getAttractorState(0)); //att1Act				| attrib: 47
			addAttribf(particleRender->_attractorUpdater.getAttractor(0).x);	//att1X					| attrib: 48
			addAttribf(particleRender->_attractorUpdater.getAttractor(0).y);	//att1Y					| attrib: 49
			addAttribf(particleRender->_attractorUpdater.getAttractor(0).z);	//att1Z					| attrib: 50
			addAttribf(particleRender->_attractorUpdater.getAttractor(0).w);	//att1W					| attrib: 51
			addAttribi(particleRender->_attractorUpdater.getAttractorState(1)); //att2Act				| attrib: 52
			addAttribf(particleRender->_attractorUpdater.getAttractor(1).x);	//att2X					| attrib: 53
			addAttribf(particleRender->_attractorUpdater.getAttractor(1).y);	//att2Y					| attrib: 54
			addAttribf(particleRender->_attractorUpdater.getAttractor(1).z);	//att2Z					| attrib: 55
			addAttribf(particleRender->_attractorUpdater.getAttractor(1).w);	//att2W					| attrib: 56
			addAttribi(particleRender->_attractorUpdater.getAttractorState(2)); //att3Act				| attrib: 57
			addAttribf(particleRender->_attractorUpdater.getAttractor(2).x);	//att3X					| attrib: 58
			addAttribf(particleRender->_attractorUpdater.getAttractor(2).y);	//att3Y					| attrib: 59
			addAttribf(particleRender->_attractorUpdater.getAttractor(2).z);	//att3Z					| attrib: 60
			addAttribf(particleRender->_attractorUpdater.getAttractor(2).w);	//att3W					| attrib: 61
			addAttribi(particleRender->_attractorUpdater.getAttractorState(3)); //att4Act				| attrib: 62
			addAttribf(particleRender->_attractorUpdater.getAttractor(3).x);	//att4X					| attrib: 63
			addAttribf(particleRender->_attractorUpdater.getAttractor(3).y);	//att4Y					| attrib: 64
			addAttribf(particleRender->_attractorUpdater.getAttractor(3).z);	//att4Z					| attrib: 65
			addAttribf(particleRender->_attractorUpdater.getAttractor(3).w);	//att4W					| attrib: 66
//X//		//===BASIC COLOUR UPDATER===//
			addAttribi(particleRender->_basicColourUpdater.isActive());			//bColUpdActive			| attrib: 67
//X//		//===POSITION COLOUR UPDATER===//
			addAttribi(particleRender->_positionColourUpdater.isActive());		//pColUpdActive			| attrib: 68
			addAttribf(particleRender->_positionColourUpdater.getMinPos().x);	//minPosColX			| attrib: 69
			addAttribf(particleRender->_positionColourUpdater.getMinPos().y);	//minPosColY			| attrib: 70
			addAttribf(particleRender->_positionColourUpdater.getMinPos().z);	//minPosColZ			| attrib: 71
			addAttribf(particleRender->_positionColourUpdater.getMinPos().w);	//minPosColW			| attrib: 72
			addAttribf(particleRender->_positionColourUpdater.getMaxPos().x);	//maxPosColX			| attrib: 73
			addAttribf(particleRender->_positionColourUpdater.getMaxPos().y);	//maxPosColY			| attrib: 74
			addAttribf(particleRender->_positionColourUpdater.getMaxPos().z);	//maxPosColZ			| attrib: 75
			addAttribf(particleRender->_positionColourUpdater.getMaxPos().w);	//maxPosColW			| attrib: 76
//X//		//===VELOCITY COLOUR UPDATER===//
			addAttribi(particleRender->_velocityColourUpdater.isActive());		//velColUpdActive		| attrib: 77
			addAttribf(particleRender->_velocityColourUpdater.getMinVel().x);	//minVelColX			| attrib: 78
			addAttribf(particleRender->_velocityColourUpdater.getMinVel().y);	//minVelColY			| attrib: 79
			addAttribf(particleRender->_velocityColourUpdater.getMinVel().z);	//minVelColZ			| attrib: 80
			addAttribf(particleRender->_velocityColourUpdater.getMinVel().w);	//minVelColW			| attrib: 81
			addAttribf(particleRender->_velocityColourUpdater.getMaxVel().x);	//maxVelColX			| attrib: 82
			addAttribf(particleRender->_velocityColourUpdater.getMaxVel().y);	//maxVelColY			| attrib: 83
			addAttribf(particleRender->_velocityColourUpdater.getMaxVel().z);	//maxVelColZ			| attrib: 84
			addAttribf(particleRender->_velocityColourUpdater.getMaxVel().w);	//maxVelColW			| attrib: 85
//X//		//===TIME UPDATER===///
			addAttribi(particleRender->_basicTimeUpdate.isActive());			//tUpdActive			| attrib: 86
//X//		//==EMITTER===//
			addAttribf(particleRender->getEmitter().getEmitScalar());			//emitScalar			| attrib: 87
			break;
		}
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


	case ComponentType::LIGHT:
		{
			SPtr_Light light = std::static_pointer_cast<Light>(_comp);
			addAttribi(light->getLightType()); // light type (0=dir, 1=point, 2=spot)
			addAttribf(light->getDiffuse().x); // Diffuse x
			addAttribf(light->getDiffuse().y); // Diffuse y
			addAttribf(light->getDiffuse().z); // Diffuse z
			addAttribf(light->getSpecular().x); // Specular x
			addAttribf(light->getSpecular().y); // Specular y
			addAttribf(light->getSpecular().z); // Specular z
			addAttribf(light->getAtteunation().x); // Constant
			addAttribf(light->getAtteunation().y); // Linear
			addAttribf(light->getAtteunation().z); // Quadratic
			break;
		}
	case ComponentType::MATERIAL:
		{
			break;
		}

		case ComponentType::CUSTOM:
		{
			SPtr_Custom custom = std::static_pointer_cast<Custom>(_comp);
			addAttribs(custom->getBehvrName()); // attrib 0 is custom class name
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
		addAttribs(compElmnt->Attribute("mesh")); // attrib 0 is model filepath
		addAttribs(compElmnt->Attribute("shader")); // attrib 1 is shader
		addAttribs(compElmnt->Attribute("textureD")); // attrib 2 is texture file path as string
		addAttribs(compElmnt->Attribute("textureS")); // attrib 2 is texture file path as string
		addAttribs(compElmnt->Attribute("textureN")); // attrib 2 is texture file path as string
		addAttribs(compElmnt->Attribute("textureH")); // attrib 2 is texture file path as string
		addAttribf(to_float(compElmnt, "tileU")); // attrib 3 is tile u
		addAttribf(to_float(compElmnt,"tileV")); // attrib 4 is tile v
		addAttribf(to_float(compElmnt,"dR")); // Diffuse x
		addAttribf(to_float(compElmnt,"dG")); // Diffuse y
		addAttribf(to_float(compElmnt,"dB")); // Diffuse z
		addAttribf(to_float(compElmnt,"sR")); // Specular x
		addAttribf(to_float(compElmnt,"sG")); // Specular y
		addAttribf(to_float(compElmnt,"sB")); // Specular z
		addAttribf(to_float(compElmnt,"specEx"));//Specular exponent
		break;
		}

	case ComponentType::ROB_REND:
		break;

	case ComponentType::PARTICLE_REND:
			addAttribi(to_int(compElmnt,"poolSize"));   	 //poolSize				| attrib:  0
			//===BOX POSITION GENERATOR===//												   
			addAttribi(to_int(compElmnt,"boxPosGenActive")); //boxPosGenActive		| attrib:  1
			addAttribf(to_float(compElmnt,"boxPosOffX"));    //boxPosOffX			| attrib:  2
			addAttribf(to_float(compElmnt,"boxPosOffY"));    //boxPosOffY			| attrib:  3
			addAttribf(to_float(compElmnt,"boxPosOffZ"));    //boxPosOffZ			| attrib:  4
			addAttribf(to_float(compElmnt,"boxPosOffW"));    //boxPosOffW			| attrib:  5																					   
			//===ROUND POSITION GENERATOR===//												   
			addAttribi(to_int(compElmnt,"boxPosGenActive")); //roundPosGenActive	| attrib:  6
			addAttribf(to_float(compElmnt,"radiusX"));		 //radiusX				| attrib:  7
			addAttribf(to_float(compElmnt,"radiusY"));		 //radiusY				| attrib:  8
			//===BASIC COLOUR GENERATOR===//
			addAttribi(to_int(compElmnt,"bColourGenActive"));//bColourGenActive		| attrib:  9
			addAttribf(to_float(compElmnt,"bMinStColX"));    //bMinStColX			| attrib: 10
			addAttribf(to_float(compElmnt,"bMinStColY"));    //bMinStColY			| attrib: 11
			addAttribf(to_float(compElmnt,"bMinStColZ"));    //bMinStColZ			| attrib: 12
			addAttribf(to_float(compElmnt,"bMinStColW"));    //bMinStColW			| attrib: 13
			addAttribf(to_float(compElmnt,"bMaxStColX"));    //bMaxStColX			| attrib: 14
			addAttribf(to_float(compElmnt,"bMaxStColY"));    //bMaxStColY			| attrib: 15
			addAttribf(to_float(compElmnt,"bMaxStColZ"));    //bMaxStColZ			| attrib: 16
			addAttribf(to_float(compElmnt,"bMaxStColW"));    //bMaxStColW			| attrib: 17
			addAttribf(to_float(compElmnt,"bMinEndColX"));   //bMinEndColX			| attrib: 18
			addAttribf(to_float(compElmnt,"bMinEndColY"));   //bMinEndColY			| attrib: 19
			addAttribf(to_float(compElmnt,"bMinEndColZ"));   //bMinEndColZ			| attrib: 20
			addAttribf(to_float(compElmnt,"bMinEndColW"));   //bMinEndColW			| attrib: 21
			addAttribf(to_float(compElmnt,"bMaxEndColX"));   //bMaxEndColX			| attrib: 22
			addAttribf(to_float(compElmnt,"bMaxEndColY"));   //bMaxEndColY			| attrib: 23
			addAttribf(to_float(compElmnt,"bMaxEndColZ"));   //bMaxEndColZ			| attrib: 24
			addAttribf(to_float(compElmnt,"bMaxEndColW"));   //bMaxEndColW			| attrib: 25
			//===BASIC VELOCITY GENERATOR===//
			addAttribi(to_int(compElmnt,"basicVelGenActive"));//basicVelGenActive	| attrib: 26
			addAttribf(to_float(compElmnt,"bMinVelX"));      //bMinVelX				| attrib: 27
			addAttribf(to_float(compElmnt,"bMinVelY"));      //bMinVelY				| attrib: 38
			addAttribf(to_float(compElmnt,"bMinVelZ"));      //bMinVelZ				| attrib: 29
			addAttribf(to_float(compElmnt,"bMinVelW"));      //bMinVelW				| attrib: 30
			addAttribf(to_float(compElmnt,"bMaxVelX"));      //bMaxVelX				| attrib: 31
			addAttribf(to_float(compElmnt,"bMaxVelY"));      //bMaxVelY				| attrib: 32
			addAttribf(to_float(compElmnt,"bMaxVelZ"));      //bMaxVelZ				| attrib: 33
			addAttribf(to_float(compElmnt,"bMaxVelW"));      //bMaxVelW				| attrib: 34
			//===SPHERE VELOCITY GENERATOR===//
			addAttribi(to_int(compElmnt,"bSphereGenActive"));//bSphereGenActive		| attrib: 35
			addAttribf(to_float(compElmnt,"bSMinVel"));      //bSMinVel				| attrib: 36
			addAttribf(to_float(compElmnt,"bSMaxVel"));      //bSMaxVel				| attrib: 37
			//===BASIC TIME GENERATOR===//
			addAttribi(to_int(compElmnt,"bTimeGenActive"));  //bTimeGenActive		| attrib: 38
			addAttribf(to_float(compElmnt,"minTime"));       //minTime				| attrib: 39
			addAttribf(to_float(compElmnt,"maxTime"));       //maxTime				| attrib: 40
			//===EULER UPDATER===//
			addAttribi(to_int(compElmnt,"eulUpdActive"));    //eulUpdActive			| attrib: 41
			addAttribf(to_float(compElmnt,"globalAccX"));    //globalAccX			| attrib: 42
			addAttribf(to_float(compElmnt,"globalAccY"));    //globalAccY			| attrib: 43
			addAttribf(to_float(compElmnt,"globalAccZ"));    //globalAccZ			| attrib: 44
			addAttribf(to_float(compElmnt,"globalAccW"));    //globalAccW			| attrib: 45
			//==ATTRACTOR UPDATER==//
			addAttribi(to_int(compElmnt,"attUpdActive"));    //attUpdActive			| attrib: 46
			addAttribi(to_int(compElmnt,"att1Act"));		 //att1Act				| attrib: 47
			addAttribf(to_float(compElmnt,"att1X"));		 //att1X				| attrib: 48
			addAttribf(to_float(compElmnt,"att1Y"));		 //att1Y				| attrib: 49
			addAttribf(to_float(compElmnt,"att1Z"));		 //att1Z				| attrib: 50
			addAttribf(to_float(compElmnt,"att1W"));		 //att1W				| attrib: 51
			addAttribi(to_int(compElmnt,"att2Act"));		 //att2Act				| attrib: 52
			addAttribf(to_float(compElmnt,"att2X"));		 //att2X				| attrib: 53
			addAttribf(to_float(compElmnt,"att2Y"));		 //att2Y				| attrib: 54
			addAttribf(to_float(compElmnt,"att2Z"));		 //att2Z				| attrib: 55
			addAttribf(to_float(compElmnt,"att2W"));		 //att2W				| attrib: 56
			addAttribi(to_int(compElmnt,"att3Act"));		 //att3Act				| attrib: 57
			addAttribf(to_float(compElmnt,"att3X"));		 //att3X				| attrib: 58
			addAttribf(to_float(compElmnt,"att3Y"));		 //att3Y				| attrib: 59
			addAttribf(to_float(compElmnt,"att3Z"));		 //att3Z				| attrib: 60
			addAttribf(to_float(compElmnt,"att3W"));		 //att3W				| attrib: 61
			addAttribi(to_int(compElmnt,"att4Act"));		 //att4Act				| attrib: 62
			addAttribf(to_float(compElmnt,"att4X"));		 //att4X				| attrib: 63
			addAttribf(to_float(compElmnt,"att4Y"));		 //att4Y				| attrib: 64
			addAttribf(to_float(compElmnt,"att4Z"));		 //att4Z				| attrib: 65
			addAttribf(to_float(compElmnt,"att4W"));		 //att4W				| attrib: 66
			//===BASIC COLOUR UPDATER===//
			addAttribi(to_int(compElmnt,"bColUpdActive"));	 //bColUpdActive		| attrib: 67
			//===POSITION COLOUR UPDATER===//
			addAttribi(to_int(compElmnt,"pColUpdActive"));	 //pColUpdActive		| attrib: 68
			addAttribf(to_float(compElmnt,"minPosColX"));	 //minPosColX			| attrib: 69
			addAttribf(to_float(compElmnt,"minPosColY"));	 //minPosColY			| attrib: 70
			addAttribf(to_float(compElmnt,"minPosColZ"));	 //minPosColZ			| attrib: 71
			addAttribf(to_float(compElmnt,"minPosColW"));	 //minPosColW			| attrib: 72
			addAttribf(to_float(compElmnt,"minPosColX"));	 //maxPosColX			| attrib: 73
			addAttribf(to_float(compElmnt,"minPosColY"));	 //maxPosColY			| attrib: 74
			addAttribf(to_float(compElmnt,"minPosColZ"));	 //maxPosColZ			| attrib: 75
			addAttribf(to_float(compElmnt,"minPosColW"));	 //maxPosColW			| attrib: 76
			//===VELOCITY COLOUR UPDATER===//
			addAttribi(to_int(compElmnt,"velColUpdActive")); //velColUpdActive		| attrib: 77
			addAttribf(to_float(compElmnt,"minVelColX"));	 //minVelColX			| attrib: 78
			addAttribf(to_float(compElmnt,"minVelColY"));	 //minVelColY			| attrib: 79
			addAttribf(to_float(compElmnt,"minVelColZ"));	 //minVelColZ			| attrib: 80
			addAttribf(to_float(compElmnt,"minVelColW"));	 //minVelColW			| attrib: 81
			addAttribf(to_float(compElmnt,"maxVelColX"));	 //maxVelColX			| attrib: 82
			addAttribf(to_float(compElmnt,"maxVelColY"));	 //maxVelColY			| attrib: 83
			addAttribf(to_float(compElmnt,"maxVelColZ"));	 //maxVelColZ			| attrib: 84
			addAttribf(to_float(compElmnt,"maxVelColW"));	 //maxVelColW			| attrib: 85
			//===TIME UPDATER===///
			addAttribi(to_int(compElmnt,"tUpdActive"));		 //tUpdActive			| attrib: 86
			//==EMITTER===//
			addAttribf(to_float(compElmnt,"emitScalar"));	 //emitScalar			| attrib: 87

		break;

	case ComponentType::PHY_BODY:
		break;

	case ComponentType::LIGHT:
		addAttribi(to_int(compElmnt,"lightType"));  // light type (0=dir, 1=point, 2=spot)
		addAttribf(to_float(compElmnt,"dR")); // Diffuse x
		addAttribf(to_float(compElmnt,"dG")); // Diffuse y
		addAttribf(to_float(compElmnt,"dB")); // Diffuse z
		addAttribf(to_float(compElmnt,"sR")); // Specular x
		addAttribf(to_float(compElmnt,"sG")); // Specular y
		addAttribf(to_float(compElmnt,"sB")); // Specular z
		addAttribf(to_float(compElmnt,"constant")); // Constant
		addAttribf(to_float(compElmnt,"linear")); // Linear
		addAttribf(to_float(compElmnt,"quadratic")); // Quadratic
		break;

	case ComponentType::MATERIAL:
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

	case ComponentType::CUSTOM:
		addAttribs(compElmnt->Attribute("behvr")); // attrib 0 is model filepath
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

			Mesh* mesh = Assets::getMesh(getStringAttrib(0)); // attrib 0 = mesh filepath
			Shader* shader = Assets::getShader(getStringAttrib(1)); // attrib 1 = shader file path
			Texture2D* textureD = Assets::getTexture(getStringAttrib(2)); // attrib 2 = diffuse texture file path
			Texture2D* textureS = Assets::getTexture(getStringAttrib(3)); // attrib 3 = specular texture file path
			Texture2D* textureN = Assets::getTexture(getStringAttrib(4)); // attrib 4 = normal texture file path
			Texture2D* textureH = Assets::getTexture(getStringAttrib(5)); // attrib 5 = height texture file path

			if(mesh == nullptr) setStringAttrib(0, "");
			if(shader == nullptr) setStringAttrib(1, "");
			if(textureD == nullptr) setStringAttrib(2, "");
			if(textureS == nullptr) setStringAttrib(3, "");
			if(textureN == nullptr) setStringAttrib(4, "");
			if(textureH == nullptr) setStringAttrib(5, "");
			// attrib 0 = model file path
			// attrib 1 = shader file path
			// attrib 2 = diffuse texture file path
			// attrib 3 = specular texture file path
			// attrib 4 = normal texture file path
			// attrib 5 = height texture file path
			// attrib 6 = uv tile U
			// attrib 7 = uv tile V

			// attrib 8 = diffuse r
			// attrib 9 = diffuse g
			// attrib 10 = diffuse b
			// attrib 11 = specular r
			// attrib 12 = specular g 
			// attrib 13 = specular b
			// attrib 14 = specular exponent
			model->setMesh(mesh); // set model
			model->setMaterial(shader, textureD,textureS,textureN, textureH, glm::vec2(getFloatAttrib(6), getFloatAttrib(7)), glm::vec3(getFloatAttrib(8),getFloatAttrib(9),getFloatAttrib(10)),glm::vec3(getFloatAttrib(11),getFloatAttrib(12),getFloatAttrib(13)),getFloatAttrib(14)); // set material with shader and texture
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

	case ComponentType::PARTICLE_REND:
		{
			SPtr_ParticleRend particleRend = std::static_pointer_cast<ParticleRenderer>(getComp());
			//--------------------KEY--------------------//
			//ORDER:
			//PARTICLE DATA	-- Particle Pool size
			//GENERATORS	-- The Emitter
			//UPDATERS		-- The Particle Update system
			//----------------END  OF KEY----------------//

			//____________START OF ATTRIBUTES____________//
			int poolSize			 =   getIntAttrib(0);							//poolSize			 | attrib:  0
			
/*X*/		bool boxPosGenActive     =   getIntAttrib(1);							//boxPosGenActive	 | attrib:  1	//===BOX POSITION GENERATOR===//		
			float boxPosOffX		 = getFloatAttrib(2);							//boxPosOffX		 | attrib:  2
			float boxPosOffY		 = getFloatAttrib(3);							//boxPosOffY		 | attrib:  3
			float boxPosOffZ		 = getFloatAttrib(4);							//boxPosOffZ		 | attrib:  4
			float boxPosOffW		 = getFloatAttrib(5);							//boxPosOffW		 | attrib:  5
			glm::vec4 boxPosOffset(boxPosOffX,boxPosOffY,boxPosOffZ,boxPosOffW);
			particleRend->_boxPosGen.setActiveFlag(boxPosGenActive);
			particleRend->_boxPosGen.setStartOffset(boxPosOffset);
			
/*X*/		bool  roundPosGenActive =    getIntAttrib(6);							//roundPosGenActive	 | attrib:  6	//===ROUND POSITION GENERATOR===//
			float radiusX			=  getFloatAttrib(7);							//roundPosOffX		 | attrib:  7
			float radiusY			=  getFloatAttrib(8);							//roundPosOffY		 | attrib:  8
			particleRend->_roundPosGen.setActiveFlag(roundPosGenActive);
			particleRend->_roundPosGen.setXRadius(radiusX);
			particleRend->_roundPosGen.setYRadius(radiusY);

/*X*/		bool  bColourGenActive  =   getIntAttrib(9);							//bColourGenActive	 | attrib: 9	//===BASIC COLOUR GENERATOR===//
			float bMinStColX        = getFloatAttrib(10);							//bMinStColX		 | attrib: 10
			float bMinStColY		= getFloatAttrib(11);							//bMinStColY		 | attrib: 11
			float bMinStColZ		= getFloatAttrib(12);							//bMinStColZ		 | attrib: 12
			float bMinStColW		= getFloatAttrib(13);							//bMinStColW		 | attrib: 13
			glm::vec4 bMinStCol(bMinStColX,bMinStColY,bMinStColZ,bMinStColW);
			particleRend->_basicColourGen.setActiveFlag(bColourGenActive);
			particleRend->_basicColourGen.setMinStartColour(bMinStCol);

			float bMaxStColX        = getFloatAttrib(14);							//bMaxStColX		 | attrib: 14
			float bMaxStColY		= getFloatAttrib(15);							//bMaxStColY		 | attrib: 15
			float bMaxStColZ		= getFloatAttrib(16);							//bMaxStColZ		 | attrib: 16
			float bMaxStColW		= getFloatAttrib(17);							//bMaxStColW		 | attrib: 17
			glm::vec4 bMaxStCol(bMaxStColX,bMaxStColY,bMaxStColZ,bMaxStColW);
			particleRend->_basicColourGen.setMaxStartColour(bMaxStCol);

			float bMinEndColX       = getFloatAttrib(18);							//bMinEndColX		 | attrib: 18
			float bMinEndColY		= getFloatAttrib(19);							//bMinEndColY		 | attrib: 19
			float bMinEndColZ		= getFloatAttrib(20);							//bMinEndColZ		 | attrib: 20
			float bMinEndColW		= getFloatAttrib(21);							//bMinEndColW		 | attrib: 21
			glm::vec4 bMinEndCol(bMinEndColX,bMinEndColY,bMinEndColZ,bMinEndColW);
			particleRend->_basicColourGen.setMinEndColour(bMinEndCol);

			float bMaxEndColX       = getFloatAttrib(22);							//bMaxEndColX		 | attrib: 22
			float bMaxEndColY		= getFloatAttrib(23);							//bMaxEndColY		 | attrib: 23
			float bMaxEndColZ		= getFloatAttrib(24);							//bMaxEndColZ		 | attrib: 24
			float bMaxEndColW		= getFloatAttrib(25);							//bMaxEndColW		 | attrib: 25
			glm::vec4 bMaxEndCol(bMaxEndColX,bMaxEndColY,bMaxEndColZ,bMaxEndColW);
			particleRend->_basicColourGen.setMaxEndColour(bMaxEndCol);

/*X*/		bool  basicVelGenActive =   getIntAttrib(26);							 //basicVelGenActive| attrib: 26	//===BASIC VELOCITY GENERATOR===//
			float bMinVelX			= getFloatAttrib(27);							 //bMinVelX			| attrib: 27
			float bMinVelY			= getFloatAttrib(28);							 //bMinVelY			| attrib: 28
			float bMinVelZ			= getFloatAttrib(29);							 //bMinVelZ			| attrib: 29
			float bMinVelW			= getFloatAttrib(30);							 //bMinVelW			| attrib: 30
			glm::vec4 bMinVel(bMinVelX,bMinVelY,bMinVelZ,bMinVelW);
			particleRend->_basicVelGen.setActiveFlag(basicVelGenActive);
			particleRend->_basicVelGen.setMinStartVel(bMinVel);

			float bMaxVelX			= getFloatAttrib(31);							//bMaxVelX			| attrib: 31
			float bMaxVelY			= getFloatAttrib(32);							//bMaxVelY			| attrib: 32 
			float bMaxVelZ			= getFloatAttrib(33);							//bMaxVelZ			| attrib: 33
			float bMaxVelW			= getFloatAttrib(34);							//bMaxVelW			| attrib: 34
			glm::vec4 bMaxVel(bMaxVelX,bMaxVelY,bMaxVelZ,bMaxVelW);
			particleRend->_basicVelGen.setMaxStartVel(bMaxVel);

/*X*/		bool  bSphereGenActive  =   getIntAttrib(35);							//bSphereGenActive	| attrib: 35	//===SPHERE VELOCITY GENERATOR===//
			float bSMinVel			= getFloatAttrib(36);							//bSMinVel			| attrib: 36
			float bSMaxVel			= getFloatAttrib(37);							//bSMaxVel			| attrib: 37
			particleRend->_sphereVelGen.setActiveFlag(bSphereGenActive);
			particleRend->_sphereVelGen.setMinVelocity(bSMinVel);
			particleRend->_sphereVelGen.setMinVelocity(bSMaxVel);

/*X*/		bool  bTimeGenActive    =   getIntAttrib(38);							//bTimeGenActive	| attrib: 38	//===BASIC TIME GENERATOR===//
			float minTime			= getFloatAttrib(39);							//minTime			| attrib: 39
			float maxTime			= getFloatAttrib(40);							//maxTime			| attrib: 40
			particleRend->_basicTimeGen.setActiveFlag(bTimeGenActive);
			particleRend->_basicTimeGen.setMinTime(minTime);
			particleRend->_basicTimeGen.setMaxTime(maxTime);

/*X*/		bool  eulUpdActive    =   getIntAttrib(41);								//eulUpdActive		| attrib: 41	//===EULER UPDATER===//
			float globalAccX		= getFloatAttrib(42);							//globalAccX		| attrib: 42
			float globalAccY		= getFloatAttrib(43);							//globalAccY		| attrib: 43
			float globalAccZ		= getFloatAttrib(44);							//globalAccZ		| attrib: 44
			float globalAccW		= getFloatAttrib(45);							//globalAccW		| attrib: 45
			glm::vec4 globalAcc(globalAccX,globalAccY,globalAccZ,globalAccW);
			particleRend->_eulerUpdater.setActiveFlag(eulUpdActive);
			particleRend->_eulerUpdater.setGlobalAcc(globalAcc);

/*X*/		bool  attUpdActive		=   getIntAttrib(46);							//attUpdActive		| attrib: 46	//==ATTRACTOR UPDATER==//
			bool  att1Act			=   getIntAttrib(47);							//att1Act			| attrib: 47
			float att1X				= getFloatAttrib(48);							//att1X				| attrib: 48
			float att1Y				= getFloatAttrib(49);							//att1Y				| attrib: 49
			float att1Z				= getFloatAttrib(50);							//att1Z				| attrib: 50
			float att1W				= getFloatAttrib(51);							//att1W				| attrib: 51
			glm::vec4 att1(att1X,att1Y,att1Z,att1W);
			particleRend->_attractorUpdater.setActiveFlag(attUpdActive);
			particleRend->_attractorUpdater.addAttractor(0,att1Act,att1);

			bool  att2Act			=   getIntAttrib(52);							//att2Act			| attrib: 52
			float att2X				= getFloatAttrib(53);							//att2X				| attrib: 53
			float att2Y				= getFloatAttrib(54);							//att2Y				| attrib: 54
			float att2Z				= getFloatAttrib(55);							//att2Z				| attrib: 55
			float att2W				= getFloatAttrib(56);							//att2W				| attrib: 56
			glm::vec4 att2(att2X,att2Y,att2Z,att2W);
			particleRend->_attractorUpdater.addAttractor(1,att2Act,att2);

			bool  att3Act			=   getIntAttrib(57);							//att3Act			| attrib: 57
			float att3X				= getFloatAttrib(58);							//att3X				| attrib: 58
			float att3Y				= getFloatAttrib(59);							//att3Y				| attrib: 59
			float att3Z				= getFloatAttrib(60);							//att3Z				| attrib: 60
			float att3W				= getFloatAttrib(61);							//att3W				| attrib: 61
			glm::vec4 att3(att3X,att3Y,att3Z,att3W);
			particleRend->_attractorUpdater.addAttractor(2,att3Act,att3);

			bool  att4Act			=   getIntAttrib(62);							//att4Act			| attrib: 62
			float att4X				= getFloatAttrib(63);							//att4X				| attrib: 63
			float att4Y				= getFloatAttrib(64);							//att4Y				| attrib: 64
			float att4Z				= getFloatAttrib(65);							//att4Z				| attrib: 65
			float att4W				= getFloatAttrib(66);							//att4W				| attrib: 66
			glm::vec4 att4(att4X,att4Y,att4Z,att4W);
			particleRend->_attractorUpdater.addAttractor(3,att4Act,att4);

/*X*/		bool  bColUpdActive		=   getIntAttrib(67);							//bColUpdActive		| attrib: 67    //===BASIC COLOUR UPDATER===//
			particleRend->_basicColourUpdater.setActiveFlag(bColUpdActive);

/*X*/		bool  pColUpdActive		=   getIntAttrib(68);							//pColUpdActive		| attrib: 68	//===POSITION COLOUR UPDATER===//
			float minPosColX		= getFloatAttrib(69);							//minPosColX		| attrib: 69	
			float minPosColY		= getFloatAttrib(70);							//minPosColY		| attrib: 70	
			float minPosColZ		= getFloatAttrib(71);							//minPosColZ		| attrib: 71	
			float minPosColW		= getFloatAttrib(72);							//minPosColW		| attrib: 72	
			glm::vec4 minPosCol(minPosColX,minPosColY,minPosColZ,minPosColW);
			particleRend->_positionColourUpdater.setActiveFlag(pColUpdActive);
			particleRend->_positionColourUpdater.setMinPos(minPosCol);

			float maxPosColX		= getFloatAttrib(73);							//maxPosColX		| attrib: 73	
			float maxPosColY		= getFloatAttrib(74);							//maxPosColY		| attrib: 74	
			float maxPosColZ		= getFloatAttrib(75);							//maxPosColZ		| attrib: 75
			float maxPosColW		= getFloatAttrib(76);							//maxPosColW		| attrib: 76	
			glm::vec4 maxPosCol(maxPosColX,maxPosColY,maxPosColZ,maxPosColW);
			particleRend->_positionColourUpdater.setMaxPos(maxPosCol);

/*X*/		bool  velColUpdActive	=   getIntAttrib(77);							//velColUpdActive	| attrib: 77	//===VELOCITY COLOUR UPDATER===//
			float minVelColX		= getFloatAttrib(78);							//minVelColX		| attrib: 78	
			float minVelColY		= getFloatAttrib(79);							//minVelColY		| attrib: 79	
			float minVelColZ		= getFloatAttrib(80);							//minVelColZ		| attrib: 80	
			float minVelColW		= getFloatAttrib(81);							//minVelColW		| attrib: 81	
			glm::vec4 minVelCol(minVelColX,minVelColY,minVelColZ,minVelColW);
			particleRend->_velocityColourUpdater.setActiveFlag(velColUpdActive);
			particleRend->_velocityColourUpdater.setMinVel(minVelCol);

			float maxVelColX		= getFloatAttrib(82);							//maxVelColX		| attrib: 82	
			float maxVelColY		= getFloatAttrib(83);							//maxVelColY		| attrib: 83	
			float maxVelColZ		= getFloatAttrib(84);							//maxVelColZ		| attrib: 84	
			float maxVelColW		= getFloatAttrib(85);							//maxVelColW		| attrib: 85	
			glm::vec4 maxVelCol(maxVelColX,maxVelColY,maxVelColZ,maxVelColW);
			particleRend->_velocityColourUpdater.setMaxVel(maxVelCol);

/*X*/		bool  tUpdActive		=   getIntAttrib(86);							//tUpdActive		| attrib: 86	//===TIME UPDATER===///
			particleRend->_basicTimeUpdate.setActiveFlag(tUpdActive);	

			float emitScalar = getFloatAttrib(87);									//emitScalar		| attrib: 87	//==EMITTER===//
			particleRend->getEmitter().setEmitScalar(emitScalar);
			particleRend->getEmitter().init(poolSize);
			particleRend->generate(poolSize);
		}
		break;

	case ComponentType::PHY_BODY:
		{
			
		}
		break;

	case ComponentType::LIGHT:
		{
			SPtr_Light light = std::static_pointer_cast<Light>(_comp);

			LightType::Type lightType = (LightType::Type)getIntAttrib(0);

			float dR = getFloatAttrib(1);
			float dG = getFloatAttrib(2);
			float dB = getFloatAttrib(3);

			float sR = getFloatAttrib(4);
			float sG = getFloatAttrib(5);
			float sB = getFloatAttrib(6);

			float constant = getFloatAttrib(7);
			float linear = getFloatAttrib(8);
			float quadratic = getFloatAttrib(9);

			
			light->setLightType(lightType);
			light->setDiffuse(glm::vec3(dR,dG,dB));
			light->setSpecular(glm::vec3(sR,sG,sB));
			light->setAtteunation(glm::vec3(constant,linear,quadratic));
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

	case ComponentType::CUSTOM:
		{
			SPtr_Custom custom = std::static_pointer_cast<Custom>(_comp);
			custom->setBehaviour(getStringAttrib(0));
		}
		break;
	}
}