#include "rendering\material.h"


Material::Material()
{
	glEnable(GL_TEXTURE_2D);
	_shader = nullptr;
	_texture = nullptr;
	_uvFraction = glm::vec2(1, 1);
}


void Material::bind(GLfloat* m, GLfloat* v, GLfloat* p)
{
	if(_shader == nullptr) return;
		
	if(_texture == nullptr)
	{
		// Unbind texture and draw without

		_shader->useProgram();
		_shader->setUniform("hasTex",false);
		_shader->setMVP(m, v, p);
		glBindTexture(GL_TEXTURE_2D, NULL); // unbind texture?? This bugs out, not sure why
		return;
	}
	else
	{
		// Bind and draw with texture
		_shader->useProgram();
		_shader->setUniform("hasTex",true);
		_shader->setMVP(m, v, p);
		_shader->setTexTile(_uvFraction);
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
	_uvFraction = tile;
}