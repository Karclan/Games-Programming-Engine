#ifndef CUSTOM_H
#define CUSTOM_H

#include <string>
#include <iostream>

#include "core\component.h"

class Behaviour;
typedef std::shared_ptr<Behaviour> SPtr_Behaviour;

class Custom : public Component
{
public:
	Custom();
	~Custom();

	ComponentType::Type getType();
	bool isOnePerObject();


	std::string getBehvrName();
	void setBehaviour(std::string behvrName); 
	void setBehaviour(SPtr_Behaviour behaviour);

	bool requestBehaviour();
	void behvrRequestMet();
	void reset();
	SPtr_Behaviour getBehaviour();

private:
	SPtr_Behaviour _behaviour;
	std::string _behvrName;
	bool _requestBehaviour; //!< Behvr name changed, request behaviour!


};

typedef std::shared_ptr<Custom> SPtr_Custom;









#endif
