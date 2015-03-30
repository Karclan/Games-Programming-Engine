#ifndef FTINTERFACE_H
#define FTINTERFACE_H

#include <iostream>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <glew\GL\glew.h>

#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>

#include <glm\gtx\matrix_transform_2d.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "rendering\shader.h"
#include "core\globals.h"
#include "core\assets.h"

struct Character
{
	GLuint		TextureID;
	glm::ivec2	Size;
	glm::ivec2	Bearing;
	GLuint		Advance;
};

class FTInterface
{
public:

	static void init();
	static void renderText(std::string, GLfloat x,GLfloat y,GLfloat scale, glm::vec3 colour);

private:

	 FTInterface();
	~FTInterface();

	static FTInterface* get();

	void genChars();
	void genVBO();

	Shader* textShader;
	GLuint VAO, VBO;
	std::map<GLchar,Character> m_Characters;
	FTInterface *m_this;
	FT_Library	 m_ftLib;
	FT_Face		 m_ftFace;
};

#endif