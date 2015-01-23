#include "light\light.h"

ComponentType::Type Light::getType()
{
	return ComponentType::LIGHT;
}
bool Light::isOnePerObject()
{
	return false;
}

