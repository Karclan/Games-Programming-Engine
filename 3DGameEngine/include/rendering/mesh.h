#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

//! The constant values in this namespace match the location of attibs in both shaders and the VBO array in the mesh class
//  I've used consts instead of an enum as they will be refered to by number in the shaders (so easier to check correct number as written explicitly).
namespace MeshAttribs
{
	const unsigned int VERT = 0;
	const unsigned int INDEX = 1;
	const unsigned int NORMAL = 2;
	const unsigned int UV = 3;
	const unsigned int COLOUR = 4;

	const unsigned int NUM_ATTRIBS = 5; // Number may change with time if I add more attribs, but never used in shader so doesn't matter
}

//! \brief Mesh

class Mesh
{
public:
	Mesh();
	~Mesh();

	void generateBuffers(); //!< Create VAO and buffer for data

	void setVerts(std::vector<glm::vec3> &verts); //!< Fill position data via vector
	void setIndices(std::vector<unsigned int> &indices); //!< Fill index data via vector
	void setNormals(std::vector<glm::vec3> &normals); //!< Fill normal data via vector
	void setUvs(std::vector<glm::vec2> &uvs); //!< Fill uv coordinate data via vector
	void setColours(std::vector<glm::vec3> &colours); //!< Fill colour data via vector

	GLuint getVao() { return _vao; }
	int numIndices() { return _dataSize[MeshAttribs::INDEX]; }


private:
	GLuint _vao; //!< The vao holding everything together!
	GLuint _buffers[MeshAttribs::NUM_ATTRIBS]; //!< To store handles for vbos / element buffer
	int _dataSize[MeshAttribs::NUM_ATTRIBS]; //!< Holds the number of elements in each buffer (useful for if you want to read from buffers later and also for drawing as we need to know how many elements)

};




#endif