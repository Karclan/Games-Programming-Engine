#ifndef ASSETS_H
#define ASSETS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <assimp\Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "rendering\primitiveShapes.h"
#include "rendering\material.h"
#include "rendering\texture2D.h"
/*! \brief Singleton class for loading and retreiving assets

	Provides functions for retreiving pointers to assets such as meshes, textures and shaders.
	Assets are requested by file name. The loader will load these assets the first time they are
	requested and store them in a map with their filename as a key. Then when the same asset is
	requested again it is retreived by this key (so it doesn't get loaded multiple times)

*/
class Assets
{
public:
	static Mesh* getPrimitiveMesh(PrimitiveShapes::Type type); //!< Get one of the hard-coded primitive shapes
	static Shader* getShader(std::string name); //!< Returns shader if loaded, loads and returns if not loaded or null if can't load. Request by name (no need for .frag or .vert suffix). Both .vert and .frag files must be in shaders folder
	static Texture2D* getTexture(std::string fileName); //!< Loads (if not loaded) and returns texture. Request by filename with extension, e.g. img.png. Texture must be stored in textures folder
	static Mesh* getMesh(std::string fileName); //!< Loads (if not loaded) and returns mesh. Request by filename with extension, e.g. mesh.obj. Meshes must be stored in models folders

	// Functions for deleting assets
	static void unloadAllAssets(); //!< Calls all unload functions
	static void unloadShaders(); //!< Delete all shaders and clear map out
	static void unloadTextures(); //!< Delete all textures and clear map out
	static void unloadMeshes(); //!< Delete all models and clear map out

	// Get pointer to map of shaders so all can be cycled through and have global uniforms set
	static const std::map<std::string, Shader*>* getShadersMap();


private:
	Assets();
	~Assets();
	Assets* _this; //! The singleton instance
	static Assets* get();
	
	void loadBasicAssets(); //!< Load assets that come as part of the engine, just the primitive shapes at the moment
	static Mesh* loadMeshFromFile(std::string &filePath);

	PrimitiveShapes _primitives;
	std::map<std::string, Shader*> _shaders;
	std::map<std::string, Texture2D*> _textures;
	std::map<std::string, Mesh*> _meshes;
	
	
	
};


#endif