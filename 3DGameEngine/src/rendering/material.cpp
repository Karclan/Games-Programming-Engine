#include "rendering\material.h"


Material::Material()
{

	glEnable(GL_TEXTURE_2D);
	_shader = nullptr;

	for(int i=0;i<4;i++)
	{
		_textures[i]=nullptr;
	}

	_uvTile = glm::vec2(1, 1);

	_diffuse = glm::vec3(1.f,1.f,1.f);
	_specular= glm::vec3(1.f,1.f,1.f);
	_specularExp = 64.f;
}

void Material::bind(glm::mat4 m, GLfloat* v, GLfloat* p)
{
	if(_shader == nullptr) return;
		
	if(_textures[DIFFUSE] == nullptr)
	{
		// Unbind texture and draw without
		_shader->useProgram();
		_shader->setUniform("material.diff",_diffuse);
		_shader->setUniform("material.spec",_specular);
		_shader->setUniform("material.specEx",_specularExp);
		_shader->setUniform("hasTex",0);
		_shader->setMVP(glm::value_ptr(m), v, p);
		_shader->setUniform("NormalMatrix",glm::mat3(glm::inverse(glm::transpose(m))));
		glBindTexture(GL_TEXTURE_2D, NULL); // unbind texture?? This bugs out, not sure why
		return;
	}
	else
	{
		// Bind and draw with texture
		_shader->useProgram();
		_shader->setUniform("material.diff",_diffuse);
		_shader->setUniform("material.spec",_specular);
		_shader->setUniform("material.specEx",_specularExp);
		_shader->setUniform("hasTex",1);
		_shader->setMVP(glm::value_ptr(m), v, p);
		_shader->setUniform("NormalMatrix",glm::mat3(glm::inverse(glm::transpose(m))));
		_shader->setTexTile(_uvTile);
		_textures[DIFFUSE]->bind(_shader, DIFFUSE);
		if(_textures[SPECUALR]!=nullptr)
		{
			_textures[SPECUALR]->bind(_shader, SPECUALR);
		}
		if(_textures[NORMAL]!=nullptr)
		{
			_textures[NORMAL]->bind(_shader, NORMAL);
		}
		if(_textures[HEIGHT]!=nullptr)
		{
			_textures[HEIGHT]->bind(_shader, HEIGHT);
		}
	}

}

void Material::setShader(Shader* shader)
{
	_shader = shader;
}

void Material::setTexture(Texture2D* texture, int i)
{
	if(texture == nullptr) return;
	_textures[i] = texture;
}

void Material::setUVTiling(glm::vec2 tile)
{
	_uvTile = tile;
}

void Material::setDiffuse(glm::vec3 diff)
{
	_diffuse = diff;
}

void Material::setSpecular(glm::vec3 spec)
{
	_specular = spec;
}

void Material::setSpecularExp(GLfloat sEx)
{
	_specularExp = sEx;
}

std::string Material::getShaderFilePath()
{
	if(_shader == nullptr) return "";
	return _shader->getFilePath();
}

std::string Material::getTextureFilePath(int i)
{
	if(_textures[i] == nullptr) return "";
	return _textures[i]->getFilePath();
}

glm::vec2 Material::getUvTile()
{
	return _uvTile;
}

glm::vec3 Material::getDiffuse()
{
	return _diffuse;
}
glm::vec3 Material::getSpecular()
{
	return _specular;
}
GLfloat Material::getSpecularExp()
{
	return _specularExp;
}