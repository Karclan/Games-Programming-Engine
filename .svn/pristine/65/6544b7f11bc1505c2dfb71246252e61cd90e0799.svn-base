#include "core\assets.h"

Assets* Assets::get()
{
	static Assets assets;
	return &assets;
}


Assets::Assets()
{
	loadBasicAssets();
}

Assets::~Assets()
{
	unloadAllAssets();
}



void Assets::loadBasicAssets()
{
	// Load the primitive shapes
	_primitives.loadShapes();
}



//-------------------------GET ASSET----------------------
// Functions for requesting (and loading if needed) assets
//--------------------------------------------------------
Mesh* Assets::getPrimitiveMesh(PrimitiveShapes::Type type)
{
	Assets* ins = Assets::get(); // get instance

	return ins->_primitives.getMesh(type);
}

Shader* Assets::getShader(std::string name)
{
	Assets* ins = Assets::get(); // get instance

	std::map<std::string, Shader*>::iterator it;
	it = ins->_shaders.find(name);
	
	if(it != ins->_shaders.end()) // then it found it!
	{
		return it->second;
	}
	else // try to load it!
	{
		Shader* newShader = new Shader();
		if(!newShader->loadFromFile(name))
		{
			std::cout << "Failed to load " << name << " shader\n";
			delete newShader;
			return nullptr;
		}
		
		// Add shader to map and return
		ins->_shaders.emplace(name, newShader);
		return newShader;
	}
}

Texture2D* Assets::getTexture(std::string fileName)
{
	Assets* ins = Assets::get(); // get instance

	std::map<std::string, Texture2D*>::iterator it;
	it = ins->_textures.find(fileName);
	
	if(it != ins->_textures.end()) // then it found it!
	{
		return it->second;
	}
	else // try to load it!
	{
		Texture2D* newTexture = new Texture2D();
		std::string filePath = "assets/textures/" + fileName;
		if(!newTexture->loadFromFile(filePath))
		{
			std::cout << "Failed to load " << fileName << " texture\n";
			delete newTexture;
			return nullptr;
		}
		
		// Add texture to map and return
		ins->_textures.emplace(fileName, newTexture);
		return newTexture;
	}
}









//-------------------UNLOAD--------------
// Functions for deleting assets
//---------------------------------------
void Assets::unloadAllAssets()
{
	unloadShaders();
	unloadTextures();
}

void Assets::unloadShaders()
{
	Assets* ins = Assets::get(); // get instance

	std::map<std::string, Shader*>::iterator it;
	for(it = ins->_shaders.begin(); it != ins->_shaders.end(); ++it)
	{
		Shader* shader = it->second;
		delete shader;
	}

	ins->_shaders.clear();
}

void Assets::unloadTextures()
{
	Assets* ins = Assets::get(); // get instance

	std::map<std::string, Texture2D*>::iterator it;
	for(it = ins->_textures.begin(); it != ins->_textures.end(); ++it)
	{
		Texture2D* texture = it->second;
		delete texture;
	}

	ins->_textures.clear();
}




const std::map<std::string, Shader*>* Assets::getShadersMap()
{ 
	Assets* ins = Assets::get(); // get instance

	return &ins->_shaders; 
}