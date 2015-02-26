#include "rendering\material.h"


Material::Material()
{

	glEnable(GL_TEXTURE_2D);
	_shader = nullptr;
	_texture = nullptr;
	_uvTile = glm::vec2(1, 1);

	_diffuse = glm::vec3(1.f,1.f,1.f);
	_specular= glm::vec3(1.f,1.f,1.f);
	_specularExp = 1.f;

	//std::cout << "It's the material guyz\n";
}
//ComponentType::Type Material::getType()
//{
//	return ComponentType::MATERIAL;
//}
//bool Material::isOnePerObject()
//{
//	return true;
//}

void Material::bind(glm::mat4 m, GLfloat* v, GLfloat* p)
{
	if(_shader == nullptr) return;
		
	if(_texture == nullptr)
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

std::string Material::getTextureFilePath()
{
	if(_texture == nullptr) return "";
	return _texture->getFilePath();
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