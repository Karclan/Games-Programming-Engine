#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <SFML\Graphics.hpp>

#include "core\asset.h"
#include "rendering\shader.h"


/*! \brief Wrapper for loading and managing a 2D texture

*/
class Texture2D : public Asset
{
public:
	bool loadFromFile(std::string filePath); //!< load from file
	void bind(Shader* shader, int textureNumber);

private:
	GLuint _texHandle; //!< Handle to texture on graphics card

};

#endif