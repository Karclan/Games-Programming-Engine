#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <glew\GL\glew.h>

#include "rendering\shader.h"
#include "rendering\texture2D.h"

#include "core\component.h"

//! \brief Material
class Material //: public Component
{
public:
	Material(); 

	enum TextureTypes{DIFFUSE=0,SPECULAR=1,NORMAL=2,HEIGHT=3};

	//ComponentType::Type getType();
	//bool isOnePerObject();

	void bind(glm::mat4 m, GLfloat* v, GLfloat* p);
	void unbind();

	void setShader(Shader* shader);
	void setTexture(Texture2D* texture, int i);
	void setUVTiling(glm::vec2 tile); //!< Set how many times image should tile

	void setDiffuse(glm::vec3 diff);
	void setSpecular(glm::vec3 spec);
	void setSpecularExp(GLfloat sEx);

	std::string getShaderFilePath();
	std::string getTextureFilePath(int i);
	glm::vec2 getUvTile();

	glm::vec3 getDiffuse();
	glm::vec3 getSpecular();
	GLfloat getSpecularExp();

private:

	Shader* _shader;
	
	Texture2D* _textures[4];

	GLuint _texHandle;
	glm::vec2 _uvTile; //!< Number of times texture repeats (assuming in wrap mode)

	glm::vec3 _diffuse;
	glm::vec3 _specular;
	GLfloat _specularExp;
};


//typedef std::shared_ptr<Material> SPtr_Material;

#endif