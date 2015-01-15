#include "rendering\shaders.h"

void Shaders::loadShaders()
{
	_shaders[DIFFUSE].loadFromFile("diffuse");
	_shaders[SPECULAR].loadFromFile("specular");
}

Shader* Shaders::getShader(Type type)
{
	return &_shaders[type];
}