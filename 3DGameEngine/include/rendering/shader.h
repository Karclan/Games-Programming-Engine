#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <string>
#include <sstream>
#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "core\asset.h"

/*! \brief Wrapper to load and manage a shader program

*/
class Shader : public Asset
{
public:
	bool loadFromFile(std::string shaderName); //!< load from file. Must have a .vert and .frag file called shaderName in the shaders folder
	void useProgram(bool useTex); //!< use this shader for drawing
	
	// I think these set functions will only work if program is current program? So should useProgram be called in these functions?
	void setMVP(GLfloat* m, GLfloat* v, GLfloat* p); //!< set the mvp matrices in the shader
	void setTexTile(glm::vec2 tile); //!< Set tex tiles
	GLuint getTexLocation() { return _texHandle; }
	
	void setDirectionalLight(GLfloat*, GLfloat*); //!< Setup lighting like this for now as I'm unsure exactly how lighting will be handled - need a few more shader lectures!


private:
	int _shaderHandle; //!< Handle to shader program

	// Handles to uniform variables that are common to all shaders - makes sense to cache them as they will be used by most if not all shaders
	GLint _modelMatrixID;
	GLint _viewMatrixID;
	GLint _projectionMatrixID;

	GLint _texHandle; //!< Handle to texture uniform location
	GLuint _texTileHandle; //!< Handle to uniform controlling tex coord tiling
	//GLuint _noTexHandle; //!< Handle to subroutine to turn textures off
	//GLuint _sampleTexHandle; //!< Handle to subroutine to turn textures on 
	GLuint _hasTexHandler; //!< Note - for setting if use texture without subroutines

	bool loadShader(std::string vFilePath, std::string fFilePath); //!< load, compile and link shader using file paths specified and return handle to shader prog
	

};

#endif