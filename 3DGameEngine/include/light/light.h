#ifndef LIGHT_H
#define LIGHT_H

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "core\component.h"
#include "core\transform.h"

/*! \brief Light component
	Illuminates the world through superiour lighting
*/

#include <iostream>
class Light : public Component
{
public:

	ComponentType::Type getType();
	bool isOnePerObject();

	void setTransform(SPtr_Transform transform){_transform = transform;}
	

private:
	SPtr_Transform _transform;

	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
};

typedef std::shared_ptr<Light> SPtr_Light;

#endif