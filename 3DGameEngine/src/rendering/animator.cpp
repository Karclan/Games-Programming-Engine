#include "rendering\animator.h"

Animator::Animator()
{
	_animation = nullptr;
	//_mesh = nullptr;
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
	
	for(int i = 0; i < Transforms.size(); ++i)
	{
		shader->setUniform(std::string("mBones["+std::to_string(i)+"]") .c_str() , Transforms[i]);
	}
}


void Animator::setMesh(Mesh* m)
{
	_mesh = m;
}

void Animator::UpdateAnim( float fDeltaTime )
{	

	BoneTransform(fDeltaTime, Transforms);
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


}

void Animator::UpdateMesh(float fDeltaTime)
{
	UpdateAnim(fDeltaTime);
	const MatrixList& animatedSkeleton = GetSkeletonMatrixList();

	for(int i=0; i < _animation->GetNumJoints(); ++i)
	{
		//dummy[i] = animatedSkeleton[i]; // * InverseBindPose[i];
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


void Animator::BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms)
{
	aiMatrix4x4 Identity; 
	Identity= aiMatrix4x4	(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);

	float deg = 120 * TimeInSeconds;
	dummy = glm::rotate(dummy, glm::radians(deg), glm::vec3(0, 1, 0));
    //Identity.InitIdentity();

    float TicksPerSecond = _animation->_iFramRate != 0 ? 
                            _animation->_iFramRate : 25.0f;
    float TimeInTicks = TimeInSeconds * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, _animation->getAnimDuration());
	
	ReadNodeHeirarchy(AnimationTime, _animation->animScene->mRootNode, Identity);
	
	Transforms.resize(_animation->GetNumJoints());
	
    for (GLint i = 0 ; i < _animation->GetNumJoints() ; i++) {
        Transforms[i] = finalTransform[i];
		//Transforms[i] = dummy;
    }
}

void Animator::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform)
{ 
	std::string NodeName(pNode->mChildren[0]->mName.data);
	
	const aiAnimation* pAnimation = _animation->animScene->mAnimations[0];

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
	

	if(_mesh->getBoneMap().find(NodeName) != _mesh->getBoneMap().end())
     {
        GLint BoneIndex =_mesh->getBoneMap()[NodeName];
		aiMatrix4x4 tempFinalTransform;
		tempFinalTransform = _mesh->getInverseTransform() * GlobalTransformation * 
                                                   _mesh->getBoneOffset()[BoneIndex];
		finalTransform[BoneIndex][0][0] = tempFinalTransform.a1;
		finalTransform[BoneIndex][0][1] = tempFinalTransform.a2;
		finalTransform[BoneIndex][0][2] = tempFinalTransform.a3;
		finalTransform[BoneIndex][0][3] = tempFinalTransform.a4;
		finalTransform[BoneIndex][1][0] = tempFinalTransform.b1;
		finalTransform[BoneIndex][1][1] = tempFinalTransform.b2;
		finalTransform[BoneIndex][1][2] = tempFinalTransform.b3;
		finalTransform[BoneIndex][1][3] = tempFinalTransform.b4;
		finalTransform[BoneIndex][2][0] = tempFinalTransform.c1;
		finalTransform[BoneIndex][2][1] = tempFinalTransform.c2;
		finalTransform[BoneIndex][2][2] = tempFinalTransform.c3;
		finalTransform[BoneIndex][2][3] = tempFinalTransform.c4;
		finalTransform[BoneIndex][3][0] = tempFinalTransform.d1;
		finalTransform[BoneIndex][3][1] = tempFinalTransform.d2;
		finalTransform[BoneIndex][3][2] = tempFinalTransform.d3;
		finalTransform[BoneIndex][3][3] = tempFinalTransform.d3;
    }
	
    for (GLint i = 0 ; i - 1< pNode->mNumChildren ; i++) {
        ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i+1], GlobalTransformation);
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

const aiNodeAnim* Animator::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
    for (GLint i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
        const std::string tempName = pAnimation->mChannels[i]->mNodeName.data;
		if (tempName == NodeName) {
            return pNodeAnim;
        }
    }
    
    return NULL;
}


GLint Animator::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);
    
    for (GLint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}

GLint Animator::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{    
    for (GLint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


GLint Animator::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (GLint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}