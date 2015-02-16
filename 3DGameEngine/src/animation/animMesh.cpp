#include "animation/animMD5.h"
#include "animation/animMesh.h"
#include <iostream>
#include <fstream>



#define BUFFER_OFFSET(i) ((char*)NULL + (i))

// Helper functions
GLenum CheckError()
{
    GLenum GLError = glGetError();
    while ( GLError != GL_NO_ERROR )
    {
        const GLubyte* errString = gluErrorString(GLError);
        std::cerr << "OpenGL Error Occured: " << errString << std::endl;

        GLError = glGetError();
    }

    return GLError;
}

void DeleteVertexBuffer( GLuint& vboID )
{
    if ( vboID != 0 )
    {
        glDeleteBuffersARB( 1, &vboID );
        vboID = 0;
    }
}

inline void CreateVertexBuffer( GLuint& vboID )
{
    // Make sure we don't loose the reference to the previous VBO if there is one
    DeleteVertexBuffer( vboID );
    glGenBuffersARB( 1, &vboID );
}

animMesh::animMesh()
: _iMD5Version(-1)
, _iNumJoints(0)
, _iNumMeshes(0)
, _bHasAnimation(false)
, _LocalToWorldMatrix(1)
, _WorldToLoacalMatrix(1)
, _VertexSkinning( VS_CPU )
{
   // _DefaultMaterial.SpecularPower = 78.0f;
}

animMesh::~animMesh()
{
    MeshList::iterator iter = _Meshes.begin();
    while ( iter != _Meshes.end() )
    {
        DestroyMesh( *iter );
        ++iter;
    }
    _Meshes.clear();
}

void animMesh::DestroyMesh( Mesh& mesh )
{
    // Delete all the VBO's
    DeleteVertexBuffer( mesh._GLPositionBuffer );
    DeleteVertexBuffer( mesh._GLNormalBuffer );
    DeleteVertexBuffer( mesh._GLTexCoord );
    DeleteVertexBuffer( mesh._GLBoneIndex );
    DeleteVertexBuffer( mesh._GLBoneWeights );
    DeleteVertexBuffer( mesh._GLIndexBuffer );
}

void animMesh::SetVertexSkinning( VertexSkinning skinning )
{
    _VertexSkinning = skinning;
}

animMesh::VertexSkinning animMesh::GetVertexSkinning() const
{
    return _VertexSkinning;
}

void animMesh::SetWorldTransform( const glm::mat4x4 world )
{
    _LocalToWorldMatrix = world;
    _WorldToLoacalMatrix = glm::inverse( _LocalToWorldMatrix );
}

glm::mat4x4 animMesh::GetWorldTransform() const
{
    return _LocalToWorldMatrix;
}

glm::mat4x4 animMesh::GetInverseWorldTransform() const
{
    return _WorldToLoacalMatrix;
}

bool animMesh::LoadModel( const std::string &filename )
{
	std::string param;
    std::string junk;   // Read junk from the file

    std::ifstream file;
	file.open(filename);
	
	//error message if file could not open
	if(!file.is_open())
	{
		std::cerr << "animMesh::LoadModel: Failed to find file: " << &filename << std::endl;
        return false;
	}


    //fs::path filePath = filename;
    // store the parent path used for loading images relative to this file.
    //fs::path parent_path = filePath.parent_path();


    int fileLength = GetFileLength( file );   
    assert( fileLength > 0 );

    _Joints.clear();
    _Meshes.clear();
    
    file >> param;

    while ( !file.eof() )
    {
        if ( param == "MD5Version" )
        {
            file >> _iMD5Version;
            assert( _iMD5Version == 10 );
        }
        else if ( param == "commandline" )
        {
            IgnoreLine(file, fileLength ); // Ignore the contents of the line
        }
        else if ( param == "numJoints" )
        {
            file >> _iNumJoints;
            _Joints.reserve(_iNumJoints);
            _AnimatedBones.assign( _iNumJoints, glm::mat4x4(1.0f) );
        }
        else if ( param == "numMeshes" )
        {
            file >> _iNumMeshes;
            _Meshes.reserve(_iNumMeshes);
        }
        else if ( param == "joints" )
        {
            Joint joint;
            file >> junk; // Read the '{' character
            for ( int i = 0; i < _iNumJoints; ++i )
            {
                file >> joint._Name >> joint._ParentID >> junk
                     >> joint._Pos.x >> joint._Pos.y >> joint._Pos.z >> junk >> junk
                     >> joint._Orient.x >> joint._Orient.y >> joint._Orient.z >> junk;
                
                RemoveQuotes( joint._Name );
                ComputeQuatW( joint._Orient );

                _Joints.push_back(joint);
                // Ignore everything else on the line up to the end-of-line character.
                IgnoreLine( file, fileLength );
            }
            file >> junk; // Read the '}' character

            BuildBindPose( _Joints );
        }
        else if ( param == "mesh" )
        {
            Mesh mesh;
            int numVerts, numTris, numWeights;

            file >> junk; // Read the '{' character
            file >> param;
            while ( param != "}" )  // Read until we get to the '}' character
            {
                if ( param == "shader" )
                {
                    file >> mesh._Shader;
                    RemoveQuotes( mesh._Shader );

					unsigned found = filename.find_last_of("/\\");

                    std::string shaderPath( mesh._Shader );
                    std::string texturePath;
					/*
                    if ( shaderPath.has_parent_path() )
                    {
                        texturePath = shaderPath;
                    }
                    else
                    {
                        texturePath = parent_path / shaderPath;
                    }
					*/
                    if ( !texturePath.find(".tga"))
                    {
                        texturePath.append( ".tga" );
                    }
					
					textureID.loadFromFile(texturePath.c_str());
                   // mesh._TexID = SOIL_load_OGL_texture( texturePath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS );
                   // mesh._Material = _DefaultMaterial;

                    file.ignore(fileLength, '\n' ); // Ignore everything else on the line
                }
                else if ( param == "numverts")
                {
                    file >> numVerts;               // Read in the vertices
                    IgnoreLine(file, fileLength);
                    for ( int i = 0; i < numVerts; ++i )
                    {
                        Vertex vert;

                        file >> junk >> junk >> junk                    // vert vertIndex (
                            >> vert._Tex0.x >> vert._Tex0.y >> junk  //  s t )
                            >> vert._StartWeight >> vert._WeightCount;

                        IgnoreLine(file, fileLength);

                        mesh._Verts.push_back(vert);
                        mesh._Tex2DBuffer.push_back(vert._Tex0);
                    }  
                }
                else if ( param == "numtris" )
                {
                    file >> numTris;
                    IgnoreLine(file, fileLength);
                    for ( int i = 0; i < numTris; ++i )
                    {
                        Triangle tri;
                        file >> junk >> junk >> tri._Indices[0] >> tri._Indices[1] >> tri._Indices[2];

                        IgnoreLine( file, fileLength );

                        mesh._Tris.push_back(tri);
                        mesh._IndexBuffer.push_back( (GLuint)tri._Indices[0] );
                        mesh._IndexBuffer.push_back( (GLuint)tri._Indices[1] );
                        mesh._IndexBuffer.push_back( (GLuint)tri._Indices[2] );
                    }              
                }
                else if ( param == "numweights" )
                {
                    file >> numWeights;
                    IgnoreLine( file, fileLength );
                    for ( int i = 0; i < numWeights; ++i )
                    {
                        Weight weight;
                        file >> junk >> junk >> weight._JointID >> weight._Bias >> junk
                            >> weight._Pos.x >> weight._Pos.y >> weight._Pos.z >> junk;

                        IgnoreLine( file, fileLength );
                        mesh._Weights.push_back(weight);
                    }
                }
                else
                {
                    IgnoreLine(file, fileLength);
                }
            
                file >> param;
            }

            PrepareMesh(mesh);
            PrepareNormals(mesh);
            CreateVertexBuffers(mesh);

            _Meshes.push_back(mesh);
        }

        file >> param;
    }

    assert( _Joints.size() == _iNumJoints );
    assert( _Meshes.size() == _iNumMeshes );

    return true;
}

bool animMesh::LoadAnim( const std::string& filename )
{
    if ( _Animation.LoadAnimation( filename ) )
    {
        // Check to make sure the animation is appropriate for this model
        _bHasAnimation = CheckAnimation( _Animation );
    }

    return _bHasAnimation;
}

bool animMesh::CheckAnimation( const animMD5& animation ) const
{
	if ( _iNumJoints != animation.GetNumJoints() )
    {
        return false;
    }

    // Check to make sure the joints match up
    for ( unsigned int i = 0; i < _Joints.size(); ++i )
    {
        const Joint& meshJoint = _Joints[i];
        const animMD5::JointInfo& animJoint = animation.GetJointInfo( i );

        if ( meshJoint._Name != animJoint._Name || 
             meshJoint._ParentID != animJoint._ParentID )
        {
            return false;
        }
    }

    return true;
}

void animMesh::BuildBindPose( const JointList& joints )
{
    _BindPose.clear();
    _InverseBindPose.clear();

    JointList::const_iterator iter = joints.begin();
    while ( iter != joints.end() )
    {
        const Joint& joint = (*iter);
        glm::mat4x4 boneTranslation = glm::translate( joint._Pos );
        glm::mat4x4 boneRotation = glm::toMat4( joint._Orient );

        glm::mat4x4 boneMatrix = boneTranslation * boneRotation;

        glm::mat4x4 inverseBoneMatrix = glm::inverse( boneMatrix );

        _BindPose.push_back( boneMatrix );
        _InverseBindPose.push_back( inverseBoneMatrix );

        ++iter;
    }
}

// Compute the position of the vertices in object local space
// in the skeleton's bind pose
bool animMesh::PrepareMesh( Mesh& mesh )
{
    mesh._PositionBuffer.clear();
    mesh._Tex2DBuffer.clear();
    mesh._BoneIndex.clear();
    mesh._BoneWeights.clear();

    // Compute vertex positions
    for ( unsigned int i = 0; i < mesh._Verts.size(); ++i )
    {
        glm::vec3 finalPos(0);
        Vertex& vert = mesh._Verts[i];

        vert._Pos = glm::vec3(0);
        vert._Normal = glm::vec3(0);
        vert._BoneWeights = glm::vec4(0);
        vert._BoneIndices = glm::vec4(0);

        // Sum the position of the weights
        for ( int j = 0; j < vert._WeightCount; ++j )
        {
            assert( j < 4 );

            Weight& weight = mesh._Weights[vert._StartWeight + j];
            Joint& joint = _Joints[weight._JointID];
            
            // Convert the weight position from Joint local space to object space
            glm::vec3 rotPos = joint._Orient * weight._Pos;

            vert._Pos += ( joint._Pos + rotPos ) * weight._Bias;
            vert._BoneIndices[j] = (float)weight._JointID;
            vert._BoneWeights[j] = weight._Bias;
        }

        mesh._PositionBuffer.push_back(vert._Pos);
        mesh._Tex2DBuffer.push_back(vert._Tex0);
        mesh._BoneIndex.push_back(vert._BoneIndices);
        mesh._BoneWeights.push_back(vert._BoneWeights);
    }

    return true;
}

// Compute the vertex normals in the Mesh's bind pose
bool animMesh::PrepareNormals( Mesh& mesh )
{
    mesh._NormalBuffer.clear();

    // Loop through all triangles and calculate the normal of each triangle
    for ( unsigned int i = 0; i < mesh._Tris.size(); ++i )
    {
        glm::vec3 v0 = mesh._Verts[ mesh._Tris[i]._Indices[0] ]._Pos;
        glm::vec3 v1 = mesh._Verts[ mesh._Tris[i]._Indices[1] ]._Pos;
        glm::vec3 v2 = mesh._Verts[ mesh._Tris[i]._Indices[2] ]._Pos;

        glm::vec3 normal = glm::cross( v2 - v0, v1 - v0 );

        mesh._Verts[ mesh._Tris[i]._Indices[0] ]._Normal += normal;
        mesh._Verts[ mesh._Tris[i]._Indices[1] ]._Normal += normal;
        mesh._Verts[ mesh._Tris[i]._Indices[2] ]._Normal += normal;
    }

    // Now normalize all the normals
    for ( unsigned int i = 0; i < mesh._Verts.size(); ++i )
    {
        Vertex& vert = mesh._Verts[i];

        vert._Normal = glm::normalize( vert._Normal );
        mesh._NormalBuffer.push_back( vert._Normal );
    }

    return true;
}

bool animMesh::PrepareMesh( Mesh& mesh, const std::vector<glm::mat4x4>& skel )
{
    for ( unsigned int i = 0; i < mesh._Verts.size(); ++i )
    {
        const Vertex& vert = mesh._Verts[i];
        glm::vec3& pos = mesh._PositionBuffer[i];
        glm::vec3& normal = mesh._NormalBuffer[i];

        pos = glm::vec3(0);
        normal = glm::vec3(0);

        for ( int j = 0; j < vert._WeightCount; ++j )
        {
            const Weight& weight = mesh._Weights[vert._StartWeight + j];
            const glm::mat4x4 boneMatrix = skel[weight._JointID];

            pos += glm::vec3( ( boneMatrix * glm::vec4( vert._Pos, 1.0f ) ) * weight._Bias );
            normal += glm::vec3( ( boneMatrix * glm::vec4( vert._Normal, 0.0f ) ) * weight._Bias );
        }
    }
    return true;
}

bool animMesh::CreateVertexBuffers( Mesh& mesh )
{
    CreateVertexBuffer( mesh._GLPositionBuffer );
    CreateVertexBuffer( mesh._GLNormalBuffer );
    CreateVertexBuffer( mesh._GLTexCoord );
    CreateVertexBuffer( mesh._GLBoneWeights );
    CreateVertexBuffer( mesh._GLBoneIndex );
    CreateVertexBuffer( mesh._GLIndexBuffer );

    // Populate the VBO's
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, mesh._GLPositionBuffer );
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(glm::vec3) * mesh._PositionBuffer.size(), &(mesh._PositionBuffer[0]), GL_STATIC_DRAW_ARB );

    glBindBufferARB( GL_ARRAY_BUFFER_ARB, mesh._GLNormalBuffer );
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(glm::vec3) * mesh._NormalBuffer.size(), &(mesh._NormalBuffer[0]), GL_STATIC_DRAW_ARB );

    glBindBufferARB( GL_ARRAY_BUFFER_ARB, mesh._GLTexCoord );
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(glm::vec2) * mesh._Tex2DBuffer.size(), &(mesh._Tex2DBuffer[0]), GL_STATIC_DRAW_ARB );

    glBindBufferARB( GL_ARRAY_BUFFER_ARB, mesh._GLBoneWeights );
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(glm::vec4) * mesh._BoneWeights.size(), &(mesh._BoneWeights[0]), GL_STATIC_DRAW_ARB );

    glBindBufferARB( GL_ARRAY_BUFFER_ARB, mesh._GLBoneIndex );
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(glm::vec4) * mesh._BoneIndex.size(), &(mesh._BoneIndex[0]), GL_STATIC_DRAW_ARB );

    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mesh._GLIndexBuffer );
    glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLuint) * mesh._IndexBuffer.size(), &(mesh._IndexBuffer[0]), GL_STATIC_DRAW_ARB ); 

    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );

    CheckError();

    return true;
}

void animMesh::Update( float fDeltaTime )
{
    if ( _bHasAnimation )
    {
        _Animation.Update(fDeltaTime);
        
        const MatrixList& animatedSkeleton = _Animation.GetSkeletonMatrixList();
        // Multiply the animated skeleton joints by the inverse of the bind pose.
        for ( int i = 0; i < _iNumJoints; ++i )
        {
            _AnimatedBones[i] = animatedSkeleton[i] * _InverseBindPose[i];
        }        
    }
    else
    {
        // No animation.. Just use identity matrix for each bone.
        _AnimatedBones.assign(_iNumJoints, glm::mat4x4(1.0) );
    }

    for ( unsigned int i = 0; i < _Meshes.size(); ++i )
    {
        // NOTE: This only needs to be done for CPU skinning, but if I want to render the
        // animated normals, I have to update the mesh on the CPU.
        PrepareMesh( _Meshes[i], _AnimatedBones );
    }
}

void animMesh::Render()
{
    glPushMatrix();
    glMultMatrixf( glm::value_ptr(_LocalToWorldMatrix) );

    // Render the meshes
    for ( unsigned int i = 0; i < _Meshes.size(); ++i )
    {
        RenderMesh( _Meshes[i] );
    }
    
    _Animation.Render();

    for ( unsigned int i = 0; i < _Meshes.size(); ++i )
    {
        RenderNormals( _Meshes[i] );
    }

    glPopMatrix();
}

void animMesh::RenderCPU( const Mesh& mesh )
{
    glColor3f( 1.0f, 1.0f, 1.0f );
	
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, mesh._TexID );

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr( mesh._Material.Ambient ) );
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr( mesh._Material.Emissive ) );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr( mesh._Material.Diffuse) );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr( mesh._Material.Specular) );
    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mesh._Material.SpecularPower );

    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, &(mesh._PositionBuffer[0]) );

    glEnableClientState( GL_NORMAL_ARRAY );
    glNormalPointer( GL_FLOAT, 0, &(mesh._NormalBuffer[0]) );

    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glTexCoordPointer( 2, GL_FLOAT, 0, &(mesh._Tex2DBuffer[0]) );

    glDrawElements( GL_TRIANGLES, mesh._IndexBuffer.size(), GL_UNSIGNED_INT, &(mesh._IndexBuffer[0]) );

    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );

    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable( GL_TEXTURE );
}

void animMesh::RenderGPU( const Mesh& mesh )
{
    //EffectManager& mgr = EffectManager::Get();
   //mgr.SetWorldMatrix( _LocalToWorldMatrix );
   //mgr.SetMaterial( mesh._Material );

   // EffectParameter& baseSampler = _Effect.GetParameterByName("baseSampler");
   // baseSampler.Set( mesh._TexID );

    //EffectParameter& boneMatrix = _Effect.GetParameterByName("gBoneMatrix");
    //boneMatrix.Set( _AnimatedBones );

   // Technique& technique = _Effect.GetFirstValidTechnique();
   // Pass& pass0 = technique.GetPassByName("p0");

   // mgr.UpdateSharedParameters();
    //_Effect.UpdateParameters();

    // Position data
    glEnableClientState( GL_VERTEX_ARRAY );
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, mesh._GLPositionBuffer );
    glVertexPointer( 3, GL_FLOAT, 0, BUFFER_OFFSET(0) );

    // Normal data
    glEnableClientState( GL_NORMAL_ARRAY );
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, mesh._GLNormalBuffer );
    glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(0) );

    // TEX0
    glActiveTextureARB( GL_TEXTURE0_ARB );
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, mesh._TexID );

    // TEXCOORD0 (Base texture coordinates)
    glClientActiveTextureARB(GL_TEXTURE0_ARB);
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, mesh._GLTexCoord );
    glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(0) );

    // TEXCOORD1 (Blend weights)
    glClientActiveTextureARB( GL_TEXTURE1_ARB );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, mesh._GLBoneWeights );
    glTexCoordPointer( 4, GL_FLOAT, 0, BUFFER_OFFSET(0) );

    // TEXCOORD2 (Bone indices)
    glClientActiveTextureARB( GL_TEXTURE2_ARB );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, mesh._GLBoneIndex );
    glTexCoordPointer( 4, GL_FLOAT, 0, BUFFER_OFFSET(0) );

    //pass0.BeginPass();

    // Draw mesh from index buffer
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mesh._GLIndexBuffer );
    glDrawElements( GL_TRIANGLES, mesh._IndexBuffer.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

   // pass0.EndPass();

    glClientActiveTextureARB( GL_TEXTURE2_ARB );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glClientActiveTextureARB( GL_TEXTURE1_ARB );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glClientActiveTextureARB( GL_TEXTURE0_ARB );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );

    glActiveTextureARB( GL_TEXTURE0_ARB );
    glDisable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, 0 );

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );

    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );

    CheckError();
}

void animMesh::RenderMesh( const Mesh& mesh )
{
    switch ( _VertexSkinning )
    {
    case VS_CPU:
        {
            RenderCPU( mesh );
        }
        break;
    case VS_GPU:
        {
            RenderGPU( mesh );
        }
        break;
    }
}

void animMesh::RenderNormals(  const Mesh& mesh )
{

    glPushAttrib( GL_ENABLE_BIT );
    glDisable( GL_LIGHTING );

    glColor3f( 1.0f, 1.0f, 0.0f );// Yellow

    glBegin( GL_LINES );
    {
        for ( unsigned int i = 0; i < mesh._PositionBuffer.size(); ++i )
        {
            glm::vec3 p0 = mesh._PositionBuffer[i];
            glm::vec3 p1 = ( mesh._PositionBuffer[i] + mesh._NormalBuffer[i] );

            glVertex3fv( glm::value_ptr(p0) );
            glVertex3fv( glm::value_ptr(p1) );
        }
    }
    glEnd();

    glPopAttrib();
}

void animMesh::RenderSkeleton( const JointList& joints )
{
    glPointSize( 5.0f );
    glColor3f( 1.0f, 0.0f, 0.0f );
    
    glPushAttrib( GL_ENABLE_BIT );

    glDisable(GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    // Draw the joint positions
    glBegin( GL_POINTS );
    {
        for ( unsigned int i = 0; i < joints.size(); ++i )
        {
            glVertex3fv( glm::value_ptr(joints[i]._Pos) );
        }
    }
    glEnd();

    // Draw the bones
    glColor3f( 0.0f, 1.0f, 0.0f );
    glBegin( GL_LINES );
    {
        for ( unsigned int i = 0; i < joints.size(); ++i )
        {
            const Joint& j0 = joints[i];
            if ( j0._ParentID != -1 )
            {
                const Joint& j1 = joints[j0._ParentID];
                glVertex3fv( glm::value_ptr(j0._Pos) );
                glVertex3fv( glm::value_ptr(j1._Pos) );
            }
        }
    }
    glEnd();

    glPopAttrib();

}
