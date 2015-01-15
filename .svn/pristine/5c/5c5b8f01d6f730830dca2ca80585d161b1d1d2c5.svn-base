#ifndef SHADERS_H
#define SHADERS_H

#include <fstream>
#include <string>
#include <sstream>
#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "rendering\shader.h"

class Shaders
{
public:
	enum Type { DIFFUSE, SPECULAR, NUM_SHADERS };

	void loadShaders();
	Shader* getShader(Type type);


private:
	Shader _shaders[NUM_SHADERS]; //!< Shader Programs

};

#endif