#pragma once;

#include "animation/Helpers.h"
#include "animMD5.h"
#include <iostream>
#include "glm/glm.hpp"
#include <vector>
#include "glew/GL/glew.h"
#include "rendering\texture2D.h"

#include "animation/Helpers.h"

class Effect;

void DeleteVertexBuffer( GLuint& vboID );

class animMesh
{
public:
    enum VertexSkinning
    {
        VS_CPU,
        VS_GPU
    };
	
    explicit animMesh();
    virtual ~animMesh();

    void SetVertexSkinning( VertexSkinning skinning );
    VertexSkinning GetVertexSkinning() const;

    bool LoadModel( const std::string& filename );
    bool LoadAnim( const std::string& filename );

    void SetWorldTransform( const glm::mat4x4 world );
    glm::mat4x4 GetWorldTransform() const;
    glm::mat4x4 GetInverseWorldTransform() const;

    void Update( float fDeltaTime );
    void Render();

protected:
    typedef std::vector<glm::vec3> PositionBuffer;
    typedef std::vector<glm::vec3> NormalBuffer;
    typedef std::vector<glm::vec2> Tex2DBuffer;
    typedef std::vector<glm::vec4> WeightBuffer;
    typedef std::vector<glm::vec4> BoneIndexBuffer;

    typedef std::vector<GLuint> IndexBuffer;

	struct Material
	{
		 Material( const glm::float4& ambient = glm::float4( 0.1f, 0.1f, 0.1f, 1.0f ), 
				   const glm::float4& emissive = glm::float4( 0.0f, 0.0f, 0.0f, 1.0f ), 
				   const glm::float4& diffuse = glm::float4( 1.0f, 1.0f, 1.0f, 1.0f ),
				   const glm::float4& specular = glm::float4( 1.0f, 1.0f, 1.0f, 1.0f ),
				   const float        specularPower = 1.0f )
				   : Ambient( ambient )
				   , Emissive( emissive )
				   , Diffuse( diffuse )
				   , Specular( specular )
				   , SpecularPower( specularPower )
					{}

		glm::float4 Ambient;
		glm::float4 Emissive;
		glm::float4 Diffuse;
		glm::float4 Specular;
		float       SpecularPower;
	};


    struct Vertex
    {
        glm::vec3   _Pos;
        glm::vec3   _Normal;
        glm::vec2   _Tex0;
        glm::vec4   _BoneWeights;
        glm::vec4   _BoneIndices;

        // DEPRICATED
        int         _StartWeight;
        int         _WeightCount;
    };
    typedef std::vector<Vertex> VertexList;

    struct Triangle
    {
        int             _Indices[3];
    };
    typedef std::vector<Triangle> TriangleList;

    struct Weight
    {
        int             _JointID;
        float           _Bias;
        glm::vec3       _Pos;
    };
    typedef std::vector<Weight> WeightList;

    struct Joint
    {
        std::string     _Name;
        int             _ParentID;
        glm::vec3       _Pos;
        glm::quat       _Orient; //without a pointer it is not allowed
    };
    typedef std::vector<Joint> JointList;

    struct Mesh
    {
        Mesh()
            : _GLPositionBuffer(0)
            , _GLNormalBuffer(0)
            , _GLBoneWeights(0)
            , _GLBoneIndex(0)
            , _GLTexCoord(0)
            , _GLIndexBuffer(0)
        {}

        std::string     _Shader;
        Material        _Material;
        // This vertex list stores the vertices's in the bind pose.
        VertexList      _Verts;
        TriangleList    _Tris;
        WeightList      _Weights;

        // A texture ID for the material
        GLuint          _TexID;

        // These buffers are used for rendering the animated mesh
        PositionBuffer  _PositionBuffer;   // Vertex position stream
        NormalBuffer    _NormalBuffer;     // Vertex normals stream
        WeightBuffer    _BoneWeights;      // Bone weights buffer
        BoneIndexBuffer _BoneIndex;       // Bone index buffer

        Tex2DBuffer     _Tex2DBuffer;      // Texture coordinate set
        IndexBuffer     _IndexBuffer;      // Vertex index buffer

        // Vertex buffer Object IDs for the vertex streams
        GLuint          _GLPositionBuffer;
        GLuint          _GLNormalBuffer;
        GLuint          _GLBoneWeights;
        GLuint          _GLBoneIndex;
        GLuint          _GLTexCoord;
        GLuint          _GLIndexBuffer;
        
    };
    typedef std::vector<Mesh> MeshList;

    // Build the bind-pose and the inverse bind-pose matrix array for the model.
    void BuildBindPose( const JointList& joints );

    // Prepare the mesh for rendering
    // Compute vertex positions and normals
    bool PrepareMesh( Mesh& mesh );
    bool PrepareMesh( Mesh& mesh, const std::vector<glm::mat4x4>& skel );
    bool PrepareNormals( Mesh& mesh );

    void DestroyMesh( Mesh& mesh );

    // Create the VBOs that are used to render the mesh with shaders
    bool CreateVertexBuffers( Mesh& mesh );

    void RenderCPU( const Mesh& mesh );
    void RenderGPU( const Mesh& mesh );

    // Render a single mesh of the model
    void RenderMesh( const Mesh& mesh );
    void RenderNormals( const Mesh& mesh );

    // Draw the skeleton of the mesh for debugging purposes.
    void RenderSkeleton( const JointList& joints );

    bool CheckAnimation( const animMD5& animation ) const;
private:
    typedef std::vector<glm::mat4x4> MatrixList;

    int                 _iMD5Version;
    int                 _iNumJoints;
    int                 _iNumMeshes;

    bool                _bHasAnimation;

    JointList           _Joints;
    MeshList            _Meshes;

    animMD5		_Animation;

    MatrixList          _BindPose;
    MatrixList          _InverseBindPose;

    // Animated bone matrix from the animation with the inverse bind pose applied.
    MatrixList          _AnimatedBones;

    glm::mat4x4         _LocalToWorldMatrix;
    glm::mat4x4         _WorldToLoacalMatrix;

    // The Cg shader effect that is used to render this model.
    //Effect&             _Effect;
    // Define a default material to meshes.
    Material            _DefaultMaterial;

    // Perform vertex skinning on the CPU or the GPU
    VertexSkinning      _VertexSkinning;

	Texture2D textureID;

};