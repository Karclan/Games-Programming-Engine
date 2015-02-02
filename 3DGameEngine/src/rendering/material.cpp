#include "rendering\material.h"


Material::Material()
{
	glEnable(GL_TEXTURE_2D);
	_shader = nullptr;
	_texture = nullptr;
	_uvTile = glm::vec2(1, 1);
}


void Material::bind(GLfloat* m, GLfloat* v, GLfloat* p)
{
	if(_shader == nullptr) return;
		
	if(_texture == nullptr)
	{
		// Unbind texture and draw without
		_shader->useProgram(false);
		_shader->setMVP(m, v, p);
		glBindTexture(GL_TEXTURE_2D, NULL); // unbind texture?? This bugs out, not sure why
		return;
	}
	else
	{
		// Bind and draw with texture
		_shader->useProgram(true);
		_shader->setMVP(m, v, p);
		_shader->setTexTile(_uvTile);
		_texture->bind(_shader, 0);
	}

}

void Material::setShader(Shader* shader)
{
	_shader = shader;
}

void Material::setTexture(Texture2D* texture)
{
	if(texture == nullptr) return;
	_texture = texture;
}

void Material::setUVTiling(glm::vec2 tile)
{
	_uvTile = tile;
}



std::string Material::getShaderFilePath()
{
	if(_shader == nullptr) return "";
	return _shader->getFilePath();
}

std::string Material::getTextureFilePath()
{
	if(_texture == nullptr) return "";
	return _texture->getFilePath();
}

glm::vec2 Material::getUvTile()
{
	return _uvTile;
}