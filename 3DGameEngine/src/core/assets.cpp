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


// Imperfect function but hey it's just for testing!
Mesh* Assets::loadMeshFromFile(std::string &filePath)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile( filePath, 
        aiProcess_CalcTangentSpace       | 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);
  
	// If the import failed, report it
	if( !scene)
	{
		std::cout << "Error Loading : " << importer.GetErrorString() << "\n";
		return nullptr;
	}
	 aiMatrix4x4 globalInverseTransform = scene->mRootNode->mTransformation;
     globalInverseTransform.Inverse();

	// Have to manually convert arrays of assimp's own Vector3 class to vectors of glm::vec3 to make it work for now 
	// Later we can implement faster method that uses the array to directly set vao (I hope...)
	std::vector<glm::vec3> verts;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> colours;
	
	// Assimp conversion here!
	aiMesh* loadedMesh = scene->mMeshes[0];

	
	std::map<std::string, int> boneMapping;

	// Lovely bone info
	struct BoneVertexInfo
	{
		int numWeights;
		glm::ivec4 boneID; 
		glm::vec4 boneWeight;

		void addWeight(int boneIDZ, float weight)
		{
			if(numWeights >= 4) return;

			boneID[numWeights] = boneIDZ;
			boneWeight[numWeights] = weight;
			numWeights +=1;
		}
	};

	std::vector<BoneVertexInfo> boneInfos;
		

	for(int i = 0; i < loadedMesh->mNumVertices; ++i)
	{
		verts.push_back(glm::vec3(loadedMesh->mVertices[i].x, loadedMesh->mVertices[i].y, loadedMesh->mVertices[i].z));
		
		if(loadedMesh->HasNormals())
			normals.push_back(glm::vec3(loadedMesh->mNormals[i].x, loadedMesh->mNormals[i].y, loadedMesh->mNormals[i].z));
		
		if(loadedMesh->HasTextureCoords(0))
			uvs.push_back(glm::vec2(loadedMesh->mTextureCoords[0][i].x, loadedMesh->mTextureCoords[0][i].y));
		
		colours.push_back(glm::vec3(1.0f, 1.0f, 1.0f));


		// Push empty values into boneID and boneWeight vectors
		BoneVertexInfo boneInfo;
		boneInfo.numWeights = 0;
		for(int j = 0; j < 4; ++j)
		{
			boneInfo.boneID[j] = 0;
			boneInfo.boneWeight[j] = 0;
		}
		boneInfos.push_back(boneInfo);
		
	}
	for(int i = 0; i < loadedMesh->mNumFaces; ++i)
	{
		for(int j = 0; j < loadedMesh->mFaces[i].mNumIndices; ++j)
		{
			indices.push_back(loadedMesh->mFaces[i].mIndices[j]);
		}
	}





	// This is where the fun starts
	for(int i = 0; i < loadedMesh->mNumBones; ++i) // for each bone
	{
		std::string boneName(loadedMesh->mBones[i]->mName.data);
		boneMapping.emplace(boneName, i);
	}

		aiMatrix4x4 BoneOffset;
		std::vector<aiMatrix4x4> boneDataOffset;

	for(int i = 0; i < loadedMesh->mNumBones; ++i) // for each bone
	{
		std::string boneName(loadedMesh->mBones[i]->mName.data);
		std::map<std::string, int>::iterator it = boneMapping.find(boneName);
		int boneID = it->second;
		
			boneDataOffset.push_back(BoneOffset);

			boneDataOffset[i] = loadedMesh->mBones[i]->mOffsetMatrix;

		// Populate BoneID and BoneWeight vectors 
		for(int j = 0; j < loadedMesh->mBones[i]->mNumWeights; ++j) // for each weight
		{
			float weight = loadedMesh->mBones[i]->mWeights[j].mWeight;


			int vertexID = loadedMesh->mBones[i]->mWeights[j].mVertexId;
			boneInfos[vertexID].addWeight(boneID, weight);

			if(weight != 0)
			{
				//std::cout << "HELLO!";
			}
		}
	}


	/*
	int bonesCount = 0;
	for(int i = 0; i < loadedMesh->mNumBones; ++i) // for each bone
	{
		std::string boneName(loadedMesh->mBones[i]->mName.data);

		// Puts in map if not there
		std::map<std::string, int>::iterator it;
		it = boneMapping.find(boneName);

		if(it == boneMapping.end()) // then it can't find it
		{
			boneMapping.emplace(boneName, bonesCount);
			bonesCount ++;
		}
		
		// Get bone index
		GLint boneIndex = boneMapping.find(boneName)->second;

		// Store mOffset matrix from Assimp aiBone class? If we need it...
		// TO DO


		// Populate BoneID and BoneWeight vectors 
		for(int j = 0; j < loadedMesh->mBones[i]->mNumWeights; ++j) // for each
		{
			int vertexID = loadedMesh->mBones[i]->mWeights[j].mVertexId;
			int weigthNum = boneInfos[vertexID].numWeights; // the number of the weight, up to 3 (coz 4 possible)
			if(weigthNum >= 4) continue; //too many bones!!!
			boneInfos[vertexID].numWeights += 1; // add one to num of weights

			boneInfos[vertexID].boneID[weigthNum] = boneIndex;
			boneInfos[vertexID].boneWeight[weigthNum] = loadedMesh->mBones[i]->mWeights[j].mWeight;
		}
	}
	*/

	
	// This is a horrible stupid way of doing it but can change once you know it works
	std::vector<glm::ivec4> boneIDs;
	std::vector<glm::vec4> boneWeights;
	for(int i = 0; i < boneInfos.size(); ++i)
	{
		boneIDs.push_back(boneInfos[i].boneID);
		boneWeights.push_back(boneInfos[i].boneWeight);
	}
	// end horrible way of doing things



	// Now set mesh properties via vector
	Mesh* mesh = new Mesh();
	mesh->generateBuffers();
	mesh->setVerts(verts);
	mesh->setIndices(indices);
	mesh->setNormals(normals);
	mesh->setUvs(uvs);
	mesh->setColours(colours);

	mesh->setBoneMap(boneMapping);
	mesh->setBones(boneIDs, boneWeights);
	mesh->setBoneOffset(boneDataOffset);
	mesh->setInverseTransform(globalInverseTransform);

	return mesh;
}



//-------------------------GET ASSET----------------------
// Functions for requesting (and loading if needed) assets
//--------------------------------------------------------
/*
Mesh* Assets::getPrimitiveMesh(PrimitiveShapes::Type type)
{
	Assets* ins = Assets::get(); // get instance

	return ins->_primitives.getMesh(type);
}
*/

Shader* Assets::getShader(std::string name)
{
	if(name == "") return nullptr;

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
		newShader->setFilePath(name);
		return newShader;
	}
}

Texture2D* Assets::getTexture(std::string fileName)
{
	if(fileName == "") return nullptr;

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
		std::string filePath = ASSETS_PATH + "textures/" + fileName;
		if(!newTexture->loadFromFile(filePath))
		{
			std::cout << "Failed to load " << fileName << " texture\n";
			delete newTexture;
			return nullptr;
		}
		
		// Add texture to map and return
		ins->_textures.emplace(fileName, newTexture);
		newTexture->setFilePath(fileName);
		return newTexture;
	}
}


Mesh* Assets::getMesh(std::string fileName)
{
	if(fileName == "") return nullptr;

	Assets* ins = Assets::get(); // get instance

	// Primitive meshes
	if(fileName == "triangle") return ins->_primitives.getMesh(PrimitiveShapes::TRIANGLE);
	if(fileName == "cube") return ins->_primitives.getMesh(PrimitiveShapes::CUBE);
	if(fileName == "sphere") return ins->_primitives.getMesh(PrimitiveShapes::SPHERE);


	std::map<std::string, Mesh*>::iterator it;
	it = ins->_meshes.find(fileName);
	
	if(it != ins->_meshes.end()) // then it found it!
	{
		return it->second;
	}
	else // try to load it!
	{
		Mesh* newMesh = nullptr;
		std::string filePath = ASSETS_PATH + "models/" + fileName;
		newMesh = loadMeshFromFile(filePath);
		if(newMesh == nullptr)
		{
			std::cout << "Failed to load " << fileName << " model\n";
			return nullptr;
		}
		
		// Add texture to map and return
		ins->_meshes.emplace(fileName, newMesh);
		newMesh->setFilePath(fileName);
		return newMesh;
	}
}

Animation* Assets::getAnim(std::string fileName)
{
	if(fileName == "") return nullptr;

	Assets* ins = Assets::get(); // get instance


	std::map<std::string, Animation*>::iterator it;
	it = ins->_anims.find(fileName);
	
	if(it != ins->_anims.end()) // then it found it!
	{
		return it->second;
	}
	else // try to load it!
	{
		Animation* newAnim = new Animation();
		std::string filePath = ASSETS_PATH + "models/" + fileName;
		if(!newAnim->LoadAnimation(filePath))
		{
			std::cout << "Failed to load " << fileName << " anim\n";
			delete newAnim;
			return nullptr;
		}
	
		// Add texture to map and return
		ins->_anims.emplace(fileName, newAnim);
		newAnim->setFilePath(fileName);
		return newAnim;
	}
}







//-------------------UNLOAD--------------
// Functions for deleting assets
//---------------------------------------
void Assets::unloadAllAssets()
{
	unloadShaders();
	unloadTextures();
	unloadMeshes();
	unloadAnims();
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


void Assets::unloadMeshes()
{
	Assets* ins = Assets::get(); // get instance

	std::map<std::string, Mesh*>::iterator it;
	for(it = ins->_meshes.begin(); it != ins->_meshes.end(); ++it)
	{
		Mesh* mesh = it->second;
		delete mesh;
	}

	ins->_meshes.clear();
}

void Assets::unloadAnims()
{
	Assets* ins = Assets::get(); // get instance

	std::map<std::string, Animation*>::iterator it;
	for(it = ins->_anims.begin(); it != ins->_anims.end(); ++it)
	{
		Animation* anim = it->second;
		delete anim;
	}

	ins->_anims.clear();
}

const std::map<std::string, Shader*>* Assets::getShadersMap()
{ 
	Assets* ins = Assets::get(); // get instance

	return &ins->_shaders; 
}