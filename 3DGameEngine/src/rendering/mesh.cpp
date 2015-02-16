#include "rendering\mesh.h"

Mesh::Mesh()
{
	_primID = -1;
}
Mesh::~Mesh(){}



void Mesh::generateBuffers()
{
	// Generate Buffers and VAO
	glGenVertexArrays(1, &_vao);
	glGenBuffers(MeshAttribs::NUM_ATTRIBS, _buffers);
	
	// Link all buffers to VAO
	glBindVertexArray(_vao); // Bind VAO we just created so we can link it to buffers
		
	// Enable and link position buffer to vao
	glEnableVertexAttribArray(MeshAttribs::VERT);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::VERT]);
	glVertexAttribPointer(MeshAttribs::VERT, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	// Enable and link index buffer to vao
	glEnableVertexAttribArray(MeshAttribs::INDEX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[MeshAttribs::INDEX]);
	glVertexAttribPointer(MeshAttribs::INDEX, 1, GL_UNSIGNED_INT, GL_FALSE, 0, (GLubyte*)NULL);

	// Enable and link normal buffer to vao
	glEnableVertexAttribArray(MeshAttribs::NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::NORMAL]);
	glVertexAttribPointer(MeshAttribs::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	// Enable and link uv buffer to vao
	glEnableVertexAttribArray(MeshAttribs::UV);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::UV]);
	glVertexAttribPointer(MeshAttribs::UV, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	// Enable and link colour buffer to vao
	glEnableVertexAttribArray(MeshAttribs::COLOUR);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::COLOUR]);
	glVertexAttribPointer(MeshAttribs::COLOUR, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	// Enable and link bone ids buffer to vao
	glEnableVertexAttribArray(MeshAttribs::BONE_IDS);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::BONE_IDS]);
	glVertexAttribPointer(MeshAttribs::BONE_IDS, 4, GL_INT, GL_FALSE, 0, (GLubyte*)NULL);

	// Enable and link bone weights buffer to vao
	glEnableVertexAttribArray(MeshAttribs::BONE_WEIGHTS);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::BONE_WEIGHTS]);
	glVertexAttribPointer(MeshAttribs::BONE_WEIGHTS, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
}



// Set the mesh attribs
void Mesh::setVerts(std::vector<glm::vec3> &verts)
{
	// Set vertex position data
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::VERT]); // bind the vertex (position) buffer so we can...
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * verts.size(), glm::value_ptr(verts[0]), GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::VERT] = verts.size();
	
}

void Mesh::setIndices(std::vector<unsigned int> &indices)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[MeshAttribs::INDEX]); // bind the index buffer so we can...
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::INDEX] = indices.size();
}

void Mesh::setNormals(std::vector<glm::vec3> &normals)
{
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::NORMAL]); // bind the normal buffer so we can...
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), glm::value_ptr(normals[0]), GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::NORMAL] = normals.size();
}

void Mesh::setUvs(std::vector<glm::vec2> &uvs)
{
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::UV]); // bind the uv buffer so we can...
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), glm::value_ptr(uvs[0]), GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::UV] = uvs.size();
}

void Mesh::setColours(std::vector<glm::vec3> &colours)
{
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::COLOUR]); // bind the colour buffer so we can...
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colours.size(), glm::value_ptr(colours[0]), GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::COLOUR] = colours.size();
}

void Mesh::setBones(std::vector<GLint [4]> &boneIds, std::vector<GLfloat [4]> &boneWeights)
{
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::BONE_IDS]); // bind the colour buffer so we can...
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_INT) * boneIds.size(), &boneIds[0], GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::BONE_IDS] = boneIds.size();

	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::BONE_WEIGHTS]); // bind the colour buffer so we can...
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_INT) * boneWeights.size(), &boneWeights[0], GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::BONE_IDS] = boneWeights.size();
	
}
