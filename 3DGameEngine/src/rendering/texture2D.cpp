#include "rendering\texture2D.h"
#include <iostream>

bool Texture2D::loadFromFile(std::string filePath)
{
	// Use SFML to load image data
	sf::Image image;
	bool ok = image.loadFromFile(filePath);
	image.flipVertically(); // they always come in upside down!

	// Generate openGL texture2D using image data
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &_texHandle);
	glBindTexture(GL_TEXTURE_2D, _texHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	glBindTexture(GL_TEXTURE_2D, 0);	

	return ok;
}

void Texture2D::bind(Shader* shader, int textureNumber)
{
	// Set texture
	glActiveTexture(GL_TEXTURE0 + textureNumber);
	glBindTexture(GL_TEXTURE_2D, _texHandle);
	switch(textureNumber)
	{
	case 0:
		shader->setUniform("u_DiffuseTexture",textureNumber);
		break;
	case 1:
		shader->setUniform("u_SpecularTexture",textureNumber);
		break;
	case 2:
		shader->setUniform("u_NormalTexture",textureNumber);
		break;
	case 3:
		shader->setUniform("u_HeightTexture",textureNumber);
		break;
	default:	
		break;
	}
	//glUniform1i(shader->getTexLocation(), textureNumber);
}