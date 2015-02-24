#include "rendering\animator.h"

Animator::Animator()
{
	_animation = nullptr;
}

ComponentType::Type Animator::getType()
{
	return ComponentType::ANIMATION;
}

bool Animator::isOnePerObject()
{
	return true;
}

void Animator::BuildFrameSkeleton( FrameSkeletonList& skeletons, const Animation::JointInfoList& jointInfos, const Animation::BaseFrameList& baseFrames, const Animation::Frame& frame )
{
    FrameSkeleton skeleton;

    for ( unsigned int i = 0; i < jointInfos.size(); ++i )
    {
        unsigned int j = 0;

        const Animation::JointInfo& jointInfo = jointInfos[i];
        // Start with the base frame position and orientation.
        SkeletonJoint animatedJoint = baseFrames[i];

        animatedJoint.m_Parent = jointInfo._parentID;

        if ( jointInfo._flags & 1 ) // Pos.x
        {
            animatedJoint.m_Pos.x = frame._frameData[ jointInfo._startIndex + j++ ];
        }
        if ( jointInfo._flags & 2 ) // Pos.y
        {
            animatedJoint.m_Pos.y = frame._frameData[ jointInfo._startIndex + j++ ];
        }
        if ( jointInfo._flags & 4 ) // Pos.x
        {
            animatedJoint.m_Pos.z  = frame._frameData[ jointInfo._startIndex + j++ ];
        }
        if ( jointInfo._flags & 8 ) // Orient.x
        {
            animatedJoint.m_Orient.x = frame._frameData[ jointInfo._startIndex + j++ ];
        }
        if ( jointInfo._flags & 16 ) // Orient.y
        {
            animatedJoint.m_Orient.y = frame._frameData[ jointInfo._startIndex + j++ ];
        }
        if ( jointInfo._flags & 32 ) // Orient.z
        {
            animatedJoint.m_Orient.z = frame._frameData[ jointInfo._startIndex + j++ ];
        }

        ComputeQuatW( animatedJoint.m_Orient );

        if ( animatedJoint.m_Parent >= 0 ) // Has a parent joint
        {
            SkeletonJoint& parentJoint = skeleton.m_Joints[animatedJoint.m_Parent];
            glm::vec3 rotPos = parentJoint.m_Orient * animatedJoint.m_Pos;

            animatedJoint.m_Pos = parentJoint.m_Pos + rotPos;
            animatedJoint.m_Orient = parentJoint.m_Orient * animatedJoint.m_Orient;

            animatedJoint.m_Orient = glm::normalize( animatedJoint.m_Orient );
        }

        skeleton.m_Joints.push_back(animatedJoint);
    }

    skeletons.push_back(skeleton);
}



ElapsedTime::ElapsedTime( float maxTimeStep /* = 0.03333f */ )
: m_fMaxTimeStep( maxTimeStep )
, m_fPrevious ( std::clock() / (float)CLOCKS_PER_SEC )
{}

float ElapsedTime::GetElapsedTime() const
{
    float fCurrentTime = std::clock() / (float)CLOCKS_PER_SEC;
    float fDeltaTime = fCurrentTime - m_fPrevious;
    m_fPrevious = fCurrentTime;

    // Clamp to the max time step
    fDeltaTime = std::min( fDeltaTime, m_fMaxTimeStep );

    return fDeltaTime;
}




