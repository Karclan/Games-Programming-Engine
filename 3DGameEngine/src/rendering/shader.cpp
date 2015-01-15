#include "rendering\shader.h"
#include <iostream>

bool Shader::loadFromFile(std::string shaderName)
{
	std::string vFilePath = "assets/shaders/" + shaderName + ".vert";
	std::string fFilePath = "assets/shaders/" + shaderName + ".frag";


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
		_noTexHandle = glGetSubroutineIndex(_shaderHandle, GL_FRAGMENT_SHADER, "noTex");
		_sampleTexHandle = glGetSubroutineIndex(_shaderHandle, GL_FRAGMENT_SHADER, "sampleTex");
		//GLuint v1 = glGetSubroutineUniformLocation(_shaderHandle, GL_FRAGMENT_SHADER, "texColour"); // this would query sub location if we were to need to set multiple subroutines

	}

	return success;
}


void Shader::useProgram(bool useTex)
{
	glUseProgram(_shaderHandle); // use the shader for current rendering
	if(useTex) glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &_sampleTexHandle);
	else glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &_noTexHandle);

}

void Shader::setMVP(GLfloat* m, GLfloat* v, GLfloat* p)
{
	// This is how to pass uniform values to the shader!
	glUniformMatrix4fv(_modelMatrixID, 1, false, m);
	glUniformMatrix4fv(_viewMatrixID, 1, false, v);
	glUniformMatrix4fv(_projectionMatrixID, 1, false, p);
}

void Shader::setTexTile(glm::vec2 tile)
{
	glUniform2fv(_texTileHandle, 1, glm::value_ptr(tile));
}

void Shader::setDirectionalLight(GLfloat* lightDir, GLfloat* intensity)
{
	GLint dirID = glGetUniformLocation(_shaderHandle, "lightDir");
	GLint intenseID = glGetUniformLocation(_shaderHandle, "lightIntens");
	if(dirID == -1 || intenseID == -1) return;

	glUseProgram(_shaderHandle); // use the shader for current rendering
	glUniform3fv(dirID, 1, lightDir);
	glUniform3fv(intenseID, 1, intensity);
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
        fprintf(stderr, "Error opening file: shader/basic.vert\n" );
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
      fprintf(stderr, "Error creating vertex shader.\n");
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
        fprintf(stderr, "Error opening file: shader/basic.frag\n" );
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