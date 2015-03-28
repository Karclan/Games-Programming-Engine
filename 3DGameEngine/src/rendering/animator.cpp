#include "rendering\animator.h"

Animator::Animator()
{
	_animation = nullptr;
	_mesh = nullptr;
	_fAnimTime = 0;
	
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

	//load dummy values for the transforms vector
	for ( int i = 0; i < _animation->GetNumJoints(); ++i )
    {
		Transforms.push_back(dummy);
    }
}

void Animator::UpdateAnim( float fDeltaTime )
{	
	
	
    if ( _animation->getNumFrames() < 1 ) return;

	_fAnimTime += fDeltaTime;

	while ( _fAnimTime > _animation->getAnimDuration() ) _fAnimTime -= _animation->getAnimDuration();
    while ( _fAnimTime < 0.0f ) _fAnimTime += _animation->getAnimDuration();

    // Figure out which frame we're on
	float fFramNum = _fAnimTime * (float)_animation->getFrameRate();
    int iFrame0 = (int)floorf( fFramNum );
    int iFrame1 = (int)ceilf( fFramNum );
	iFrame0 = iFrame0 % _animation->getNumFrames();
    iFrame1 = iFrame1 % _animation->getNumFrames();

	float fInterpolate = fmodf( _fAnimTime, _animation->getFrameDuration() ) /  _animation->getFrameDuration();

	InterpolateSkeletons( _animation->GetSkeleton(), _animation->getSkeletons()[iFrame0], _animation->getSkeletons()[iFrame1], fInterpolate );
}

void Animator::Update(float fDeltaTime)
{
		float deg = 120 * fDeltaTime;
		dummy = glm::rotate(dummy, glm::radians(deg), glm::vec3(0, 1, 0));
       
      
        // Multiply the animated skeleton joints by the inverse of the bind pose.
		 if ( _animation )
		 {
			 UpdateAnim(fDeltaTime);
				const MatrixList& animatedSkeleton = _animation->GetSkeletonMatrixList();
				if(_mesh){
				glm::mat4 tempInverse = _mesh->getInverseTransform();
				
				// Multiply the animated skeleton joints by the inverse of the bind pose.
				for ( int i = 0; i < _animation->GetNumJoints(); ++i )
				{
					
					//Transforms[i] = animatedSkeleton[i] * _mesh->getInverseTransform();
					//Transforms[i]=(animatedSkeleton[i] * tempInverse);
				}    
				}
		 }else{
			 // No animation.. Just use identity matrix for each bone.
			 Transforms.assign(_animation->GetNumJoints(), glm::mat4x4(1.0) );
		 }	

}


void Animator::InterpolateSkeletons( Animation::FrameSkeleton& finalSkeleton, const Animation::FrameSkeleton& skeleton0, const Animation::FrameSkeleton& skeleton1, float fInterpolate )
{
	
	for ( int i = 0; i < _animation->GetNumJoints(); ++i )
    {
        Animation::SkeletonJoint& finalJoint = finalSkeleton.m_Joints[i];
        glm::mat4x4& finalMatrix = finalSkeleton.m_BoneMatrices[i];

        const Animation::SkeletonJoint& joint0 = skeleton0.m_Joints[i];
        const Animation::SkeletonJoint& joint1 = skeleton1.m_Joints[i];

        finalJoint.m_Parent = joint0.m_Parent;

        finalJoint.m_Pos = glm::lerp( joint0.m_Pos, joint1.m_Pos, fInterpolate );
        finalJoint.m_Orient = glm::mix( joint0.m_Orient, joint1.m_Orient, fInterpolate );

        
        finalMatrix = glm::translate( finalJoint.m_Pos ) * glm::toMat4( finalJoint.m_Orient );
		//temp check
		Transforms[i] = finalMatrix;
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

