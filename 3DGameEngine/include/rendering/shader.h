#ifndef SHADER_H
#define SHADER_H


#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

#include "core\asset.h"
#include "core\globals.h"

/*! \brief Wrapper to load and manage a shader program

*/
class Shader : public Asset
{
public:
	bool loadFromFile(std::string shaderName); //!< load from file. Must have a .vert and .frag file called shaderName in the shaders folder
	void useProgram(); //!< use this shader for drawing
	
	// I think these set functions will only work if program is current program? So should useProgram be called in these functions?
	void setMVP(GLfloat* m, GLfloat* v, GLfloat* p); //!< set the mvp matrices in the shader
	void setTexTile(glm::vec2 tile); //!< Set tex tiles
	GLuint getTexLocation() { return _texHandle; }

	void setUniform(const char *name, float x, float y, float z);
    void setUniform(const char *name, const glm::vec2	&v);
    void setUniform(const char *name, const glm::vec3	&v);
    void setUniform(const char *name, const glm::vec4	&v);
    void setUniform(const char *name, const glm::mat4	&m);
    void setUniform(const char *name, const glm::mat3	&m);
    void setUniform(const char *name, float				val );
    void setUniform(const char *name, int				val );
    void setUniform(const char *name, bool				val );
    void setUniform(const char *name, GLuint			val );
	void setUniform(const char *name, GLfloat		*mat);

	/*void setUniformS(std::string &name, float x, float y, float z);
    void setUniformS(std::string &name, const glm::vec2	&v);
    void setUniformS(std::string &name, const glm::vec3	&v);
    void setUniformS(std::string &name, const glm::vec4	&v);
    void setUniformS(std::string &name, const glm::mat4	&m);
    void setUniformS(std::string &name, const glm::mat3	&m);
    void setUniformS(std::string &name, float		 val );
    void setUniformS(std::string &name, int		 val );
    void setUniformS(std::string &name, bool		 val );
    void setUniformS(std::string &name, GLuint		 val );
	void setUniformS(std::string &name, GLfloat *mat);*/
	

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

	std::map<std::string, int> _uniformLocations;//<! All the active uniform locations
	
	bool	loadShader(std::string vFilePath, std::string fFilePath); //!< load, compile and link shader using file paths specified and return handle to shader prog
	GLint   getUniformLocation(const char * name );//!< Returns uniform location if active, or finds it if not

	glm::vec3 lightPos[3];
};

#endif