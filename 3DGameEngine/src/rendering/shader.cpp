#include "rendering\shader.h"


bool Shader::loadFromFile(std::string shaderName)
{
	std::string vFilePath = ASSETS_PATH + "shaders/" + shaderName + ".vert";
	std::string fFilePath = ASSETS_PATH + "shaders/" + shaderName + ".frag";

	bool success = loadShader(vFilePath, fFilePath);

	if(success)
	{
		// Note that glGetUniformLocation returns -1 if can't find uniform
		// This could be very useful later for ensuring shaders still work even if certain uniforms aren't present

		// This is how to get reference to information from the shader
		_modelMatrixID = glGetUniformLocation(_shaderHandle, "mModel");
		_viewMatrixID = glGetUniformLocation(_shaderHandle, "mView");
		_projectionMatrixID = glGetUniformLocation(_shaderHandle, "mProjection");

		// set texture uniforms - I think I can just set main tex to 0 and leave as is, then always use 0 for main tex
		// Then have a system for others later on as well, e.g. 1 is always bump map etc
		_texHandle = glGetUniformLocation(_shaderHandle, "tex"); // main texture
		_texTileHandle = glGetUniformLocation(_shaderHandle, "uvTile"); // texture fraction multiplier

		// For subroutines. Currently relies on fact that only 1 subroutine as setting multiple is confusing as hell!
		//_noTexHandle = glGetSubroutineIndex(_shaderHandle, GL_FRAGMENT_SHADER, "noTex");
		//_sampleTexHandle = glGetSubroutineIndex(_shaderHandle, GL_FRAGMENT_SHADER, "sampleTex");
		//GLuint v1 = glGetSubroutineUniformLocation(_shaderHandle, GL_FRAGMENT_SHADER, "texColour"); // this would query sub location if we were to need to set multiple subroutines
		_hasTexHandler = glGetUniformLocation(_shaderHandle, "hasTex");

	}

	return success;
}


void Shader::useProgram()
{
	glUseProgram(_shaderHandle); // use the shader for current rendering

	/*
	if(useTex) glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &_sampleTexHandle);
	else glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &_noTexHandle);
	*/
}

void Shader::setMVP(GLfloat* m, GLfloat* v, GLfloat* p)
{
	// This is how to pass uniform values to the shader!
	setUniform("mModel",m);
	setUniform("mView",v);
	setUniform("mProjection",p);
}

void Shader::setTexTile(glm::vec2 tile)
{
	setUniform("uvTile",tile);
}

void Shader::setDirectionalLight()
{
	glUseProgram(_shaderHandle); // use the shader for current rendering
}

void Shader::setUniform(const char *name, float x, float y, float z)
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc,x,y,z);
}

void Shader::setUniform(const char *name, const glm::vec3 &v)
{
	this->setUniform(name,v.x,v.y,v.z);
}

void Shader::setUniform(const char *name, const glm::vec4 &v)
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc,v.x,v.y,v.z,v.w);
}

void Shader::setUniform(const char *name, const glm::vec2 &v)
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc,v.x,v.y);
}
void Shader::setUniform(const char *name, const glm::mat4 & m)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const char *name, const glm::mat3 & m)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const char *name, float val )
{
	GLint loc = getUniformLocation(name);
	glUniform1f(loc, val);
}

void Shader::setUniform(const char *name, int val )
{
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, val);
}

void Shader::setUniform(const char *name, GLuint val )
{
	GLint loc = getUniformLocation(name);
	glUniform1ui(loc, val);
}
void Shader::setUniform(const char *name, GLfloat *val)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc, 1, false, val);
}
void Shader::setUniform(const char *name, bool val )
{
	int loc = getUniformLocation(name);
	glUniform1i(loc, val);
}

//void Shader::setUniform(const std::string &name, float x, float y, float z)
//{
//	GLint loc = getUniformLocation(&name[0]);
//	glUniform3f(loc,x,y,z);
//}
//
//void Shader::setUniform(const std::string &name, const glm::vec3 &v)
//{
//	this->setUniform(&name[0],v.x,v.y,v.z);
//}
//
//void Shader::setUniform(const std::string &name, const glm::vec4 &v)
//{
//	GLint loc = getUniformLocation(&name[0]);
//	glUniform4f(loc,v.x,v.y,v.z,v.w);
//}
//
//void Shader::setUniform(const std::string &name, const glm::vec2 &v)
//{
//	GLint loc = getUniformLocation(&name[0]);
//	glUniform2f(loc,v.x,v.y);
//}
//void Shader::setUniform(const std::string &name, const glm::mat4 & m)
//{
//	GLint loc = getUniformLocation(&name[0]);
//	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
//}
//
//void Shader::setUniform(const std::string &name, const glm::mat3 & m)
//{
//	GLint loc = getUniformLocation(&name[0]);
//	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
//}
//
//void Shader::setUniform(const std::string &name, float val )
//{
//	GLint loc = getUniformLocation(&name[0]);
//	glUniform1f(loc, val);
//}
//
//void Shader::setUniform(const std::string &name, int val )
//{
//	GLint loc = getUniformLocation(&name[0]);
//	glUniform1i(loc, val);
//}
//
//void Shader::setUniform(const std::string &name, GLuint val )
//{
//	GLint loc = getUniformLocation(&name[0]);
//	glUniform1ui(loc, val);
//}
//void Shader::setUniform(const std::string &name, GLfloat *val)
//{
//	GLint loc = getUniformLocation(&name[0]);
//	glUniformMatrix4fv(loc, 1, false, val);
//}
//void Shader::setUniform(const std::string &name, bool val )
//{
//	int loc = getUniformLocation(&name[0]);
//	glUniform1i(loc, val);
//}

GLint Shader::getUniformLocation(const char *name)
{
	std::map<std::string, int>::iterator pos;
	pos = _uniformLocations.find(name);

	if( pos == _uniformLocations.end() ) {
		_uniformLocations[name] = glGetUniformLocation(_shaderHandle, name);
	}

	return _uniformLocations[name];
}






bool Shader::loadShader(std::string vFilePath, std::string fFilePath)
{
	// LOAD SHADERS VIA STRING STREAMING

	//////////////////////////////////////////////////////
	/////////// Vertex shader //////////////////////////
	//////////////////////////////////////////////////////


	// Load contents of file
	std::ifstream inFile(vFilePath);
	if (!inFile) // if file bad then error
	{
		std::cout<<"Error opening file: "+vFilePath+" \n";
		return false;
	}

	// Pull data from shader prog to string stream then store in string
	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	std::string codeStr(code.str()); // code is stored in this string

	// Create the shader object
	GLuint vertShader = glCreateShader( GL_VERTEX_SHADER );
	if (0 == vertShader) 
	{
		std::cout<<"Error creating file: "+vFilePath+" \n";
		return false;
	}

	// Load the source code into the shader object
	const GLchar* codeArray[] = {codeStr.c_str()};
	glShaderSource(vertShader, 1, codeArray, NULL); // attach source code from string to shader with ID vertShader
	//const GLchar *codeArray = reinterpret_cast<const GLchar*>(codeStr.c_str());
	//glShaderSource(vertShader, 1, &codeArray, NULL); // attach source code from string to shader with ID vertShader

	// Compile the shader
	glCompileShader( vertShader );

	// Check compilation status
	GLint result;
	glGetShaderiv( vertShader, GL_COMPILE_STATUS, &result );
	if( GL_FALSE == result ) 
	{
		fprintf( stderr, "Vertex shader compilation failed!\n" );

		GLint logLen;
		glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &logLen );

		if (logLen > 0)
		{
			char * log = (char *)malloc(logLen);

			GLsizei written;
			glGetShaderInfoLog(vertShader, logLen, &written, log);

			fprintf(stderr, "Shader log: \n%s", log);

			free(log);
		}
	}


	//////////////////////////////////////////////////////
	/////////// Fragment shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file into shaderCode here
	std::ifstream fragFile(fFilePath);
	if (!fragFile)
	{
		std::cout<<"Error creating file: "+fFilePath+" \n";
		return false;
	}

	std::stringstream fragCode;
	fragCode << fragFile.rdbuf();
	fragFile.close();
	codeStr = fragCode.str();

	// Create the shader object
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (0 == fragShader) 
	{
		fprintf(stderr, "Error creating fragment shader.\n");
		return false;
	}

	// Load the source code into the shader object
	codeArray[0] = codeStr.c_str();
	glShaderSource( fragShader, 1, codeArray, NULL );
	//codeArray = reinterpret_cast<const GLchar*>(codeStr.c_str());
	//glShaderSource(fragShader, 1, &codeArray, NULL); // attach source code from string to shader with ID vertShader

	// Compile the shader
	glCompileShader( fragShader );

	// Check compilation status
	glGetShaderiv( fragShader, GL_COMPILE_STATUS, &result );
	if (GL_FALSE == result) 
	{
		fprintf( stderr, "Fragment shader compilation failed!\n" );

		GLint logLen;
		glGetShaderiv( fragShader, GL_INFO_LOG_LENGTH, &logLen );

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			glGetShaderInfoLog(fragShader, logLen, &written, log);

			fprintf(stderr, "Shader log: \n%s", log);

			free(log);
		}
	}

	//----------------------------------------------------------
	// NOW LINK SHADER
	// Create the program object
	_shaderHandle = glCreateProgram();
	if(0 == _shaderHandle) 
	{
		fprintf(stderr, "Error creating program object.\n");
		return false;
	}


	// Attach the shaders to the program object
	glAttachShader( _shaderHandle, vertShader );
	glAttachShader( _shaderHandle, fragShader );

	// Link the program
	glLinkProgram( _shaderHandle );

	// Check for successful linking
	GLint status;
	glGetProgramiv( _shaderHandle, GL_LINK_STATUS, &status );

	if (GL_FALSE == status)
	{

		fprintf( stderr, "Failed to link shader program!\n" );

		GLint logLen;
		glGetProgramiv( _shaderHandle, GL_INFO_LOG_LENGTH, &logLen );

		if (logLen > 0)
		{
			char * log = (char *)malloc(logLen);

			GLsizei written;
			glGetProgramInfoLog(_shaderHandle, logLen, &written, log);

			fprintf(stderr, "Program log: \n%s", log);

			free(log);
		}
	}

	return true;
}

