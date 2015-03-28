#include "rendering\light.h"

Light::Light()
{
	_type = LightType::POINT; // just made this default for now

	setDepFlag(ComponentType::TRANSFORM);

	_diffuse.x =0.5;
	_diffuse.y =0.5;
	_diffuse.z =0.5;

	_specular.x =1;
	_specular.y =1;
	_specular.z =1;

	_atteunation.x =1;
	_atteunation.y =0.014;
	_atteunation.z =0.0007;

	//std::cout<<"light created!\n";
}
ComponentType::Type Light::getType()
{
	return ComponentType::LIGHT;
}
bool Light::isOnePerObject()
{
	return false;
}

void Light::linkDependency(SPtr_Component component)
{
	switch(component->getType())
	{
	case ComponentType::TRANSFORM:
		_transform = std::static_pointer_cast<Transform>(component);
		break;

	}
}
void Light::setLightType(LightType::Type type)
{
	_type=type;
}
void Light::setDiffuse(glm::vec3 diff)
{
	_diffuse = diff;
}
void Light::setSpecular(glm::vec3 spec)
{
	_specular = spec;
}
void Light::setAtteunation(glm::vec3 atten)
{
	_atteunation = atten;
}