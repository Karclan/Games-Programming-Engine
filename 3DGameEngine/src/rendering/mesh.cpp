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
}



/// Set the mesh attribs
void Mesh::setVerts(std::vector<glm::vec3> &verts)
{
	if(verts.size() == 0) return;

	glBindVertexArray(_vao); // Bind VAO so we can link it to buffers

	// Enable and link position buffer to vao
	glEnableVertexAttribArray(MeshAttribs::VERT);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::VERT]);
	glVertexAttribPointer(MeshAttribs::VERT, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	// Set vertex position data
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::VERT]); // bind the vertex (position) buffer so we can...
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * verts.size(), glm::value_ptr(verts[0]), GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::VERT] = verts.size();
	
	glBindVertexArray(0); // Unbind VAO
}

void Mesh::setIndices(std::vector<unsigned int> &indices)
{
	if(indices.size() == 0) return;

	glBindVertexArray(_vao); // Bind VAO so we can link it to buffers

	// Enable and link index buffer to vao
	glEnableVertexAttribArray(MeshAttribs::INDEX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[MeshAttribs::INDEX]);
	glVertexAttribPointer(MeshAttribs::INDEX, 1, GL_UNSIGNED_INT, GL_FALSE, 0, (GLubyte*)NULL);

	// Set index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[MeshAttribs::INDEX]); // bind the index buffer so we can...
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::INDEX] = indices.size();

	glBindVertexArray(0); // Unbind VAO
}

void Mesh::setNormals(std::vector<glm::vec3> &normals)
{
	if(normals.size() == 0) return;

	glBindVertexArray(_vao); // Bind VAO so we can link it to buffers

	// Enable and link normal buffer to vao
	glEnableVertexAttribArray(MeshAttribs::NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::NORMAL]);
	glVertexAttribPointer(MeshAttribs::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	// Set normal data
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::NORMAL]); // bind the normal buffer so we can...
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), glm::value_ptr(normals[0]), GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::NORMAL] = normals.size();

	glBindVertexArray(0); // Unbind VAO
}

void Mesh::setUvs(std::vector<glm::vec2> &uvs)
{
	if(uvs.size() == 0) return;

	glBindVertexArray(_vao); // Bind VAO so we can link it to buffers

	// Enable and link uv buffer to vao
	glEnableVertexAttribArray(MeshAttribs::UV);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::UV]);
	glVertexAttribPointer(MeshAttribs::UV, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	// Set uv data
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::UV]); // bind the uv buffer so we can...
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), glm::value_ptr(uvs[0]), GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::UV] = uvs.size();

	glBindVertexArray(0); // Unbind VAO
}

void Mesh::setColours(std::vector<glm::vec3> &colours)
{
	if(colours.size() == 0) return;

	glBindVertexArray(_vao); // Bind VAO so we can link it to buffers

	// Enable and link colour buffer to vao
	glEnableVertexAttribArray(MeshAttribs::COLOUR);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::COLOUR]);
	glVertexAttribPointer(MeshAttribs::COLOUR, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	// Set colour data 
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::COLOUR]); // bind the colour buffer so we can...
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colours.size(), glm::value_ptr(colours[0]), GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::COLOUR] = colours.size();

	glBindVertexArray(0); // Unbind VAO
}

void Mesh::setBones(std::vector<glm::ivec4> &boneIds, std::vector<glm::vec4> &boneWeights)
{
	if(boneIds.size() == 0 || boneWeights.size()) return;

	glBindVertexArray(_vao); // Bind VAO so we can link it to buffers

	// Enable and link bone ids buffer to vao
	glEnableVertexAttribArray(MeshAttribs::BONE_IDS);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::BONE_IDS]);
	glVertexAttribPointer(MeshAttribs::BONE_IDS, 4, GL_INT, GL_FALSE, 0, (GLubyte*)NULL);

	// Enable and link bone weights buffer to vao
	glEnableVertexAttribArray(MeshAttribs::BONE_WEIGHTS);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::BONE_WEIGHTS]);
	glVertexAttribPointer(MeshAttribs::BONE_WEIGHTS, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);


	// Set boneID data
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::BONE_IDS]); // bind the colour buffer so we can...
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_INT) * boneIds.size(), &boneIds[0], GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::BONE_IDS] = boneIds.size();

	// Set bone weight data
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[MeshAttribs::BONE_WEIGHTS]); // bind the colour buffer so we can...
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_INT) * boneWeights.size(), &boneWeights[0], GL_STATIC_DRAW); // ..populate it with the data
	_dataSize[MeshAttribs::BONE_IDS] = boneWeights.size();
	
	glBindVertexArray(0); // Unbind VAO
}

void Mesh::BuildBindPose( const JointList& joints )
{
    m_BindPose.clear();
    m_InverseBindPose.clear();

    JointList::const_iterator iter = joints.begin();
    while ( iter != joints.end() )
    {
        const Joint& joint = (*iter);
        glm::mat4x4 boneTranslation = glm::translate( joint.m_Pos );
        glm::mat4x4 boneRotation = glm::toMat4( joint.m_Orient );

        glm::mat4x4 boneMatrix = boneTranslation * boneRotation;

        glm::mat4x4 inverseBoneMatrix = glm::inverse( boneMatrix );

        m_BindPose.push_back( boneMatrix );
        m_InverseBindPose.push_back( inverseBoneMatrix );

        ++iter;
    }
}

void Mesh::setMap(std::map<std::string, int> m)
{
	boneMap = m;
}

///// Need to get the verts position based on the weights //////

/*
bool Mesh::PrepareMesh( Mesh& mesh )
{

	// Compute vertex positions
    for ( unsigned int i = 0; i < mesh.m_Verts.size(); ++i )
    {
        glm::vec3 finalPos(0);
        Vertex& vert = mesh.m_Verts[i];

        vert.m_Pos = glm::vec3(0);
        vert.m_Normal = glm::vec3(0);
        vert.m_BoneWeights = glm::vec4(0);
        vert.m_BoneIndices = glm::vec4(0);

        // Sum the position of the weights
        for ( int j = 0; j < vert.m_WeightCount; ++j )
        {
            assert( j < 4 );

            Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
            Joint& joint = m_Joints[weight.m_JointID];
            
            // Convert the weight position from Joint local space to object space
            glm::vec3 rotPos = joint.m_Orient * weight.m_Pos;

            vert.m_Pos += ( joint.m_Pos + rotPos ) * weight.m_Bias;
            vert.m_BoneIndices[j] = (float)weight.m_JointID;
            vert.m_BoneWeights[j] = weight.m_Bias;
        }

        mesh.m_PositionBuffer.push_back(vert.m_Pos);
        mesh.m_Tex2DBuffer.push_back(vert.m_Tex0);
        mesh.m_BoneIndex.push_back(vert.m_BoneIndices);
        mesh.m_BoneWeights.push_back(vert.m_BoneWeights);
    }

    return true;
}
*/

//// Compute the vertex normals in the Mesh's bind pose
/*
bool Mesh::PrepareNormals( Mesh& mesh )
{
   // mesh.m_NormalBuffer.clear();

    // Loop through all triangles and calculate the normal of each triangle
    for ( unsigned int i = 0; i < mesh.m_Tris.size(); ++i )
    {
        glm::vec3 v0 = mesh.m_Verts[ mesh.m_Tris[i].m_Indices[0] ].m_Pos;
        glm::vec3 v1 = mesh.m_Verts[ mesh.m_Tris[i].m_Indices[1] ].m_Pos;
        glm::vec3 v2 = mesh.m_Verts[ mesh.m_Tris[i].m_Indices[2] ].m_Pos;

        glm::vec3 normal = glm::cross( v2 - v0, v1 - v0 );

        mesh.m_Verts[ mesh.m_Tris[i].m_Indices[0] ].m_Normal += normal;
        mesh.m_Verts[ mesh.m_Tris[i].m_Indices[1] ].m_Normal += normal;
        mesh.m_Verts[ mesh.m_Tris[i].m_Indices[2] ].m_Normal += normal;
    }

    // Now normalize all the normals
    for ( unsigned int i = 0; i < mesh.m_Verts.size(); ++i )
    {
        Vertex& vert = mesh.m_Verts[i];

        vert.m_Normal = glm::normalize( vert.m_Normal );
        mesh.m_NormalBuffer.push_back( vert.m_Normal );
    }

    return true;
}
*/