#include "core\component.h"
#include <iostream>

Component::Component()
{
	_dependencyFlags = 0;
	_lifeCycleState = ComponentState::ACTIVE;
}

Component::~Component()
{	
}

BITMASK Component::getDepFlags()
{
	return _dependencyFlags;
}

BITMASK Component::getOptionalDepFlags()
{
	return _optionalDepFlags;
}



void Component::destroy()
{
	_lifeCycleState = ComponentState::DESTROYED;
}




void Component::setDepFlag(ComponentType::Type type)
{
	// 1 shifted by component's index in enum will produce a bit mask of all zeros except the flag representing this component
	// E.g. 0000000000000001 = Component 0
	//      0000000000000010 = Component 1 etc

	// By performing | on this mask with component flags, all 0 values will result remain unchanged because 0 | 0 == 0 and 0 | 1 == 1
	// Therefore the only bit that may be changed is the bit in the mask set to 1, which represents the component ID in enum
	// If it is 0 then 0 | 1 = 1, so it will be set
	// If it is 1 then 1 | 1 = 1, so it will be set

	_dependencyFlags = _dependencyFlags | 1<<(int)type;
}


bool Component::getIfDepFlagSet(ComponentType::Type type)
{
	// 1 shifted by component's index in enum will produce a bit mask of all zeros except the flag representing this component
	// E.g. 0000000000000001 = Component 0
	//      0000000000000010 = Component 1 etc

	// By performing & on this mask with component flags, all 0 values will result in a zero because 0 & 0 == 0 and 0 & 1 == 0
	// Therefore the only bit that may come back 1 is the bit in the mask set to 1, which represents the component ID in enum
	// If it is 0 then the flag is not set and therefore all bits will be 0. Cast to a bool gives you false
	// If it is 1 then the flag is set and this value will & with 1 in the bitmask to produce 1, giving us some number other than 0. Cast to bool gives you true!

	return _dependencyFlags & 1<<(int)type;
}



void Component::setOptionalDepFlag(ComponentType::Type type)
{
	_optionalDepFlags = _optionalDepFlags | 1<<(int)type;
}