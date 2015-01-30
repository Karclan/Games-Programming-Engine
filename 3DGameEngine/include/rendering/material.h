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
	void bind(GLfloat* m, GLfloat* v, GLfloat* p);
	void setUVTiling(glm::vec2 tile); //!< Set how many times image should tile

	std::string getShaderFilePath();
	std::string getTextureFilePath();
	glm::vec2 getUvTile();


private:
	Shader* _shader;
	Texture2D* _texture;
	GLuint _texHandle;
	glm::vec2 _uvTile; //!< Number of times texture repeats (assuming in wrap mode)
};

#endif