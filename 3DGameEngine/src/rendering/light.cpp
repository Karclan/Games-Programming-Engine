#include "rendering\light.h"

Light::Light()
{
	setDepFlag(ComponentType::TRANSFORM);
	_ambient.x =0;
	_ambient.y =0;
	_ambient.z =0;

	_diffuse.x =0;
	_diffuse.y =0;
	_diffuse.z =0;

	_specular.x =0;
	_specular.y =0;
	_specular.z =0;

	_atteunation.x =0;
	_atteunation.y =0;
	_atteunation.z =0;

	std::cout<<"light created!\n";
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
void Light::setAmbient(glm::vec3 amb)
{
	_ambient = amb;
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