#include "rendering\animator.h"

Animator::Animator()
{
	_animation = nullptr;
	animTime = _animation->getAnimTime();
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

void Animator::Update( float fDeltaTime )
{
	
    if ( _animation->getNumFrames() < 1 ) return;

	animTime += fDeltaTime;
	float animTime = _animation->getAnimTime();

	while ( animTime >_animation->getAnimDuration() ) animTime -= _animation->getAnimDuration();
    while ( animTime < 0.0f ) animTime += _animation->getAnimDuration();

    // Figure out which frame we're on
	float fFramNum = animTime * (float)_animation->getFrameRate();
    int iFrame0 = (int)floorf( fFramNum );
    int iFrame1 = (int)ceilf( fFramNum );
	iFrame0 = iFrame0 % _animation->getNumFrames();
    iFrame1 = iFrame1 % _animation->getNumFrames();

	float fInterpolate = fmodf( animTime, _animation->getFrameDuration() ) /  _animation->getFrameDuration();

    //InterpolateSkeletons( m_AnimatedSkeleton, m_Skeletons[iFrame0], m_Skeletons[iFrame1], fInterpolate );
}

void Animator::InterpolateSkeletons( FrameSkeleton& finalSkeleton, const FrameSkeleton& skeleton0, const FrameSkeleton& skeleton1, float fInterpolate )
{
    for ( int i = 0; i < _animation->GetNumJoints(); ++i )
    {
        SkeletonJoint& finalJoint = finalSkeleton.m_Joints[i];
        glm::mat4x4& finalMatrix = finalSkeleton.m_BoneMatrices[i];

        const SkeletonJoint& joint0 = skeleton0.m_Joints[i];
        const SkeletonJoint& joint1 = skeleton1.m_Joints[i];

        finalJoint.m_Parent = joint0.m_Parent;

        finalJoint.m_Pos = glm::lerp( joint0.m_Pos, joint1.m_Pos, fInterpolate );
        finalJoint.m_Orient = glm::mix( joint0.m_Orient, joint1.m_Orient, fInterpolate );

        // Build the bone matrix for GPU skinning.
        finalMatrix = glm::translate( finalJoint.m_Pos ) * glm::toMat4( finalJoint.m_Orient );
    }
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




