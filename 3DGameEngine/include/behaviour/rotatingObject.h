#ifndef ROTATING_OBJECT_h
#define ROTATING_OBJECT_h

#include "behaviour\behaviour.h"
#include "core\input.h"

/*! \brief Temporary behaviour class

	Hard-code behaviour for controlling an object and making it rotate around the
	y axis. In final version this would be a script file.

*/

class RotatingObject : public Behaviour
{
public:
	RotatingObject(){};
	~RotatingObject(){};

protected:
	void initialize();
	void fixedUpdate(float t); //!< Update function

private:
	std::shared_ptr<Transform> _transform;

};


/*! \brief Temporary behaviour class

	Rotate object manually, for debugging transform class

*/

class ManualRotater : public Behaviour
{
public:
	ManualRotater(){};
	~ManualRotater(){};

protected:
	void initialize();
	void update(float t);
	void fixedUpdate(float t); //!< Update function

private:
	std::shared_ptr<Transform> _transform;
	float _axisH;
	float _axisV;

};







#endif