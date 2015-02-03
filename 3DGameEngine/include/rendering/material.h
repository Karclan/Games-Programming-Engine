#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <glew\GL\glew.h>

#include "rendering\shader.h"
#include "rendering\texture2D.h"

//! \brief Material
class Material
{
public:
	Material(); 

	void setShader(Shader* shader);
	void setTexture(Texture2D* texture);
	void bind(glm::mat4 m, GLfloat* v, GLfloat* p);
	void setUVTiling(glm::vec2 tile); //!< Set how many times image should tile

	void setDiffuse(glm::vec3 diff);
	void setSpecular(glm::vec3 spec);
	void setSpecularExp(GLfloat sEx);

	std::string getShaderFilePath();
	std::string getTextureFilePath();
	glm::vec2 getUvTile();

	glm::vec3 getDiffuse();
	glm::vec3 getSpecular();
	GLfloat getSpecularExp();

private:
	Shader* _shader;
	Texture2D* _texture;
	GLuint _texHandle;
	glm::vec2 _uvTile; //!< Number of times texture repeats (assuming in wrap mode)

	glm::vec3 _diffuse;
	glm::vec3 _specular;
	GLfloat _specularExp;
};

#endif