#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "core\component.h"
#include "rendering\shader.h"
#include "core\transform.h"
#include "rendering\animation.h"
#include "rendering/mesh.h"

#include <iostream>
#include <fstream>
#include <ctime>

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
#include <assimp\Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/matrix4x4.h>

class Animator : public Component
{
public:
	Animator();

	typedef std::vector<glm::mat4x4> SkeletonMatrixList;
	 // A Skeleton joint is a joint of the skeleton per frame
    struct SkeletonJoint
    {
        SkeletonJoint()
            : m_Parent(-1)
            , m_Pos(0)
        {}
 
        SkeletonJoint( const Animation::BaseFrame& copy )
            : m_Pos( copy._position )
			, m_Orient( copy._orientation )
        {}
 
        int         m_Parent;
        glm::vec3   m_Pos;
        glm::quat   m_Orient;
    };
    typedef std::vector<SkeletonJoint> SkeletonJointList;
 
    // A frame skeleton stores the joints of the skeleton for a single frame.
    struct FrameSkeleton
    {
		SkeletonMatrixList  m_BoneMatrices;
        SkeletonJointList   m_Joints;
    };
    typedef std::vector<FrameSkeleton> FrameSkeletonList;
 
    const FrameSkeleton& GetSkeleton() const
    {
        return _AnimatedSkeleton;
    }

	   const SkeletonMatrixList& GetSkeletonMatrixList() const
    {
		return _AnimatedSkeleton.m_BoneMatrices;
    }

	ComponentType::Type getType(); //!< Required implementation. Return type of component
	bool isOnePerObject(); //!< Required implementation. Return true if you can only have one of these per object
	void UpdateAnim( float fDeltaTime );
	void UpdateMesh( float fDeltaTime );
	void bind(Shader* shader);

	glm::mat4 BoneTransform(float timeSeconds, std::vector<glm::mat4>& Transforms);
	void BuildFrameSkeleton( FrameSkeletonList& skeletons, const Animation::JointInfoList& jointInfos, const Animation::BaseFrameList& baseFrames, const Animation::Frame& frame );
	void InterpolateSkeletons( FrameSkeleton& finalSkeleton, const FrameSkeleton& skeleton0, const FrameSkeleton& skeleton1, float fInterpolate );
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode,const aiMatrix4x4& ParentTransform);
	void setBones(std::vector<GLint [4]> &boneIds, std::vector<GLfloat [4]> &boneWeights); //!< Fill bone data via vector

	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
    GLint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    GLint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    GLint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

	void setAnimation(Animation* anim) 
	{ 
		if(anim != nullptr)
		{
			_animation = anim; 
		}
	}
	Animation* getAnimation(){return _animation;}
protected:

	FrameSkeletonList   _Skeletons;    // All the skeletons for all the frames
    FrameSkeleton       _AnimatedSkeleton;

private:
	Animation* _animation;
	float animTime;
	typedef std::vector<glm::mat4x4> MatrixList;

	MatrixList _AnimatedBones;
};

//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<Animator> SPtr_Animator;

// Remove the quotes from a string
void RemoveQuotes( std::string& str );

// Get's the size of the file in bytes.
int GetFileLength( std::istream& file );

// Ignore everything else that comes on the line, up to 'length' characters.
void IgnoreLine( std::istream& file, int length );

// Computes the W component of the quaternion based on the X, Y, and Z components.
// This method assumes the quaternion is of unit length.
void ComputeQuatW( glm::quat& quat );

// Helper class to count frame time
class ElapsedTime
{
public:
    ElapsedTime( float maxTimeStep = 0.03333f );
    float GetElapsedTime() const;

private:
    float m_fMaxTimeStep;
    mutable float m_fPrevious;
};

#endif