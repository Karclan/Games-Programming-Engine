#ifndef MESH_H
#define MESH_H

#include <vector>
#include <map>

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include "core\asset.h"

//! The constant values in this namespace match the location of attibs in both shaders and the VBO array in the mesh class
//  I've used consts instead of an enum as they will be refered to by number in the shaders (so easier to check correct number as written explicitly).
namespace MeshAttribs
{
	const unsigned int VERT = 0;
	const unsigned int INDEX = 1;
	const unsigned int NORMAL = 2;
	const unsigned int UV = 3;
	const unsigned int COLOUR = 4;
	const unsigned int BONE_IDS = 5;
	const unsigned int BONE_WEIGHTS = 6;

	const unsigned int NUM_ATTRIBS = 7; // Number may change with time if I add more attribs, but never used in shader so doesn't matter
}


	typedef std::vector<GLint> WeightBuffer;
	typedef std::vector<float> BoneIndexBuffer;
	
//#define BONES_PER_VERTEX 4	/************** HARD CODED - Number of bones that can affect a vertex ******************/
struct Vertex
{
	glm::vec3   m_Pos;
    glm::vec3   m_Normal;
    glm::vec2   m_Tex0;
    glm::vec4   m_BoneWeights;
    glm::vec4   m_BoneIndices;
};	typedef std::vector<Vertex> VertexList;

struct Triangle
{
	int m_Indices[3];
};	typedef std::vector<Triangle> TriangleList;

struct Joint
{
	std::string m_Name;
	int			m_ParentID;
	glm::vec3	m_Pos;
	glm::quat	m_Orient;
}; typedef std::vector<Joint> JointList;




//! \brief Mesh

class Mesh : public Asset
{
public:
	Mesh();
	~Mesh();

	
	void SetWorldTransform( const glm::mat4x4 world );
    glm::mat4x4 GetWorldTransform() const;
    glm::mat4x4 GetInverseWorldTransform() const;

	void generateBuffers(); //!< Create VAO and buffer for data

	void setVerts(std::vector<glm::vec3> &verts); //!< Fill position data via vector
	void setIndices(std::vector<unsigned int> &indices); //!< Fill index data via vector
	void setNormals(std::vector<glm::vec3> &normals); //!< Fill normal data via vector
	void setUvs(std::vector<glm::vec2> &uvs); //!< Fill uv coordinate data via vector
	void setColours(std::vector<glm::vec3> &colours); //!< Fill colour data via vector
	void setBones(std::vector<glm::ivec4> &boneIds, std::vector<glm::vec4> &boneWeights); //!< Fill bone data via vector

	GLuint getVao() { return _vao; }
	int numIndices() { return _dataSize[MeshAttribs::INDEX]; }

	void setPrimID(int id) { _primID = id; }
	bool getPrimID() { return _primID; }

	void BuildBindPose( const JointList& joints );
	bool PrepareMesh(Mesh& mesh);
	bool PrepareNormals( Mesh& mesh );
	void update(float fDeltaTime);
	void setMap(std::map<std::string, int> m);
	std::map<std::string, int> getBoneMap() {return boneMap;}

private:
	GLuint _vao; //!< The vao holding everything together!
	GLuint _buffers[MeshAttribs::NUM_ATTRIBS]; //!< To store handles for vbos / element buffer
	int _dataSize[MeshAttribs::NUM_ATTRIBS]; //!< Holds the number of elements in each buffer (useful for if you want to read from buffers later and also for drawing as we need to know how many elements)
	int _primID; //!< If prim shape, the ID of the primitive. If not, returns -1 (so can be used to determine if primitive)

	typedef std::vector<glm::mat4x4> MatrixList;

	MatrixList m_BindPose;
	MatrixList m_InverseBindPose;
	MatrixList m_AnimatedBones;

	glm::mat4x4 m_LocalToWorldMatrix;
	glm::mat4x4 m_WorldToLocalMatrix;
	
	std::map<std::string, int> boneMap;
};

#endif