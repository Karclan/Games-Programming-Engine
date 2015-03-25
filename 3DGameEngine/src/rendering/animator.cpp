#include "rendering\animator.h"

Animator::Animator()
{
	_animation = nullptr;
	animTime = 0;
}

ComponentType::Type Animator::getType()
{
	return ComponentType::ANIMATION;
}

bool Animator::isOnePerObject()
{
	return true;
}


void Animator::bind(Shader* shader)
{

	for(int i = 0; i < 7; ++i)
	{
		shader->setUniform(std::string("mBones["+std::to_string(i)+"]") .c_str() , dummy);
	}

	for(int i = 7; i < 15; ++i)
	{
		shader->setUniform(std::string("mBones["+std::to_string(i)+"]") .c_str() , glm::mat4());
	}

	for(int i = 15; i < 100; ++i)
	{
		shader->setUniform(std::string("mBones["+std::to_string(i)+"]") .c_str() , -dummy);
	}

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

void Animator::UpdateAnim( float fDeltaTime )
{
	float deg = 120 * fDeltaTime;
	dummy = glm::rotate(dummy, glm::radians(deg), glm::vec3(0, 1, 0));
	return;



	
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

    InterpolateSkeletons( _AnimatedSkeleton, _Skeletons[iFrame0], _Skeletons[iFrame1], fInterpolate );
}

void Animator::UpdateMesh(float fDeltaTime)
{
	UpdateAnim(fDeltaTime);
	const MatrixList& animatedSkeleton = GetSkeletonMatrixList();

	for(int i=0; i < _animation->GetNumJoints(); ++i)
	{
		//_animatedBones[i] = animatedSkeleton[i] * InverseBindPose[i];
	}
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

/*
glm::mat4 Animator::BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms)
{
	glm::mat4 Identity; Identity= glm::mat4	(0.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 0.0f, 0.0f);
    //Identity.InitIdentity();

    float TicksPerSecond = _animation->_iFramRate != 0 ? 
                            _animation->_iFramRate : 25.0f;
    float TimeInTicks = TimeInSeconds * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, _animation->getAnimDuration());

    ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

    Transforms.resize(m_NumBones);

    for (GLint i = 0 ; i < m_NumBones ; i++) {
        Transforms[i] = m_BoneInfo[i].FinalTransformation;
    }
}

void Animator::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
{ 
    std::string NodeName(pNode->mName.data);

    const aiAnimation* pAnimation = _animation;

    aiMatrix4x4  NodeTransformation(pNode->mTransformation);

    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

    if (pNodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
        aiMatrix4x4 ScalingM;
		ScalingM.a1 = Scaling.x; ScalingM.b2 = Scaling.y; ScalingM.c3 = Scaling.z; ScalingM.d4 = 1.0f;

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim); 
       aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());

        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
        aiMatrix4x4 TranslationM;
		TranslationM.a4 = Translation.x; TranslationM.a1 = 1.0f;
		TranslationM.b4 = Translation.y; TranslationM.b2 = 1.0f;
		TranslationM.c4 = Translation.z; TranslationM.c3 = 1.0f; TranslationM.d4 = 1.0f;
        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM * ScalingM;
    }

    aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;
	
    if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
        uint BoneIndex = m_BoneMapping[NodeName];
        m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * 
                                                    m_BoneInfo[BoneIndex].BoneOffset;
    }

    for (GLint i = 0 ; i < pNode->mNumChildren ; i++) {
        ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
    }
}

void Animator::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }
            
    GLint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    GLint NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}


void Animator::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }
    
    GLint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    GLint NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;    
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}


void Animator::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    GLint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    GLint NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}
*/