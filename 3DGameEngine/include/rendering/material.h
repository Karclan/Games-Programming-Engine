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


private:
	Shader* _shader;
	Texture2D* _texture;
	GLuint _texHandle;
	glm::vec2 _uvFraction; //!< Multiply uv coords by this fraction, creating tiling effect (if wrap enabled)
};

#endif