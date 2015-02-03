
#include "animation/md5Anim.h"
#include <string>
#include <iostream>
#include <fstream>

MD5Animation::MD5Animation()
: _iMD5Version( 0 )
, _iNumFrames( 0 )
, _iNumJoints( 0 )
, _iFramRate( 0 )
, _iNumAnimatedComponents( 0 )
, _fAnimDuration( 0 )
, _fFrameDuration( 0 )
, _fAnimTime( 0 )
{

}

MD5Animation::~MD5Animation()
{}

bool MD5Animation::LoadAnimation( const std::string& filename )
{
    
	
	
	std::ifstream file;
	file.open(filename);

    std::string param;
    std::string junk;   // Read junk from the file

	//error message if file could not open
	if(!file.is_open())
	{
		std::cerr << "MD5Anim::LoadAnimation: Failed to find file: " << &filename << std::endl;
        return false;
	}

    int fileLength = GetFileLength( file );   
    assert( fileLength > 0 );

    _JointInfos.clear();
    _Bounds.clear();
    _BaseFrames.clear();
    _Frames.clear();
    _AnimatedSkeleton._Joints.clear();

    file >> param;

    while( !file.eof() )
    {
        if ( param == "MD5Version" )
        {
            file >> _iMD5Version;
            assert( _iMD5Version == 10 );
        }
        else if ( param == "commandline" )
        {
            file.ignore( fileLength, '\n' ); // Ignore everything else on the line
        }
        else if ( param == "numFrames" )
        {
            file >> _iNumFrames;
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "numJoints" )
        {
            file >> _iNumJoints;
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "frameRate" )
        {
            file >> _iFramRate;
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "numAnimatedComponents" )
        {
            file >> _iNumAnimatedComponents;
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "hierarchy" )
        {
            file >> junk; // read in the '{' character
            for ( int i = 0; i < _iNumJoints; ++i )
            {
                JointInfo joint;
                file >> joint._Name >> joint._ParentID >> joint._Flags >> joint._StartIndex;
                RemoveQuotes( joint._Name );

                _JointInfos.push_back(joint);

                file.ignore( fileLength, '\n' );
            }
            file >> junk; // read in the '}' character
        }
        else if ( param == "bounds" )
        {
            file >> junk; // read in the '{' character
            file.ignore( fileLength, '\n' );
            for ( int i = 0; i < _iNumFrames; ++i ) 
            {
                Bound bound;
                file >> junk; // read in the '(' character
                file >> bound._Min.x >> bound._Min.y >> bound._Min.z;
                file >> junk >> junk; // read in the ')' and '(' characters.
                file >> bound._Max.x >> bound._Max.y >> bound._Max.z;
                
                _Bounds.push_back(bound);

                file.ignore( fileLength, '\n' );
            }

            file >> junk; // read in the '}' character
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "baseframe" )
        {
            file >> junk; // read in the '{' character
            file.ignore( fileLength, '\n' );

            for ( int i = 0; i < _iNumJoints; ++i )
            {
                BaseFrame baseFrame;
                file >> junk;
                file >> baseFrame._Pos.x >> baseFrame._Pos.y >> baseFrame._Pos.z;
                file >> junk >> junk;
                file >> baseFrame._Orient.x >> baseFrame._Orient.y >> baseFrame._Orient.z;
                file.ignore( fileLength, '\n' );

                _BaseFrames.push_back(baseFrame);
            }
            file >> junk; // read in the '}' character
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "frame" )
        {
            FrameData frame;
            file >> frame._iFrameID >> junk; // Read in the '{' character
            file.ignore(fileLength, '\n' );

            for ( int i = 0; i < _iNumAnimatedComponents; ++i )
            {
                float frameData;
                file >> frameData;
                frame._FrameData.push_back(frameData);
            }

            _Frames.push_back(frame);

            // Build a skeleton for this frame
            BuildFrameSkeleton( _Skeletons, _JointInfos, _BaseFrames, frame );

            file >> junk; // Read in the '}' character
            file.ignore(fileLength, '\n' );        
        }

        file >> param;
    }

    // Make sure there are enough joints for the animated skeleton.
    _AnimatedSkeleton._Joints.assign(_iNumJoints, SkeletonJoint() );     // DEPRICATED
    _AnimatedSkeleton._BoneMatrices.assign( _iNumJoints, glm::mat4x4(1.0) );

    _fFrameDuration = 1.0f / (float)_iFramRate;
    _fAnimDuration = ( _fFrameDuration * (float)_iNumFrames );
    _fAnimTime = 0.0f;

    assert( _JointInfos.size() == _iNumJoints );
    assert( _Bounds.size() == _iNumFrames );
    assert( _BaseFrames.size() == _iNumJoints );
    assert( _Frames.size() == _iNumFrames );
    assert( _Skeletons.size() == _iNumFrames );

    return true;
}

void MD5Animation::BuildFrameSkeleton( FrameSkeletonList& skeletons, const JointInfoList& jointInfos, const BaseFrameList& baseFrames, const FrameData& frameData )
{
    FrameSkeleton skeleton;

    for ( unsigned int i = 0; i < jointInfos.size(); ++i )
    {
        unsigned int j = 0;

        const JointInfo& jointInfo = jointInfos[i];
        // Start with the base frame position and orientation.
        SkeletonJoint animatedJoint = baseFrames[i];

        animatedJoint._Parent = jointInfo._ParentID;

        if ( jointInfo._Flags & 1 ) // Pos.x
        {
            animatedJoint._Pos.x = frameData._FrameData[ jointInfo._StartIndex + j++ ];
        }
        if ( jointInfo._Flags & 2 ) // Pos.y
        {
            animatedJoint._Pos.y = frameData._FrameData[ jointInfo._StartIndex + j++ ];
        }
        if ( jointInfo._Flags & 4 ) // Pos.x
        {
            animatedJoint._Pos.z  = frameData._FrameData[ jointInfo._StartIndex + j++ ];
        }
        if ( jointInfo._Flags & 8 ) // Orient.x
        {
            animatedJoint._Orient.x = frameData._FrameData[ jointInfo._StartIndex + j++ ];
        }
        if ( jointInfo._Flags & 16 ) // Orient.y
        {
            animatedJoint._Orient.y = frameData._FrameData[ jointInfo._StartIndex + j++ ];
        }
        if ( jointInfo._Flags & 32 ) // Orient.z
        {
            animatedJoint._Orient.z = frameData._FrameData[ jointInfo._StartIndex + j++ ];
        }

        ComputeQuatW( animatedJoint._Orient );

        if ( animatedJoint._Parent >= 0 ) // Has a parent joint
        {
            SkeletonJoint& parentJoint = skeleton._Joints[animatedJoint._Parent];
            glm::vec3 rotPos = parentJoint._Orient * animatedJoint._Pos;

            animatedJoint._Pos = parentJoint._Pos + rotPos;
            animatedJoint._Orient = parentJoint._Orient * animatedJoint._Orient;

            animatedJoint._Orient = glm::normalize( animatedJoint._Orient );
        }
        
        skeleton._Joints.push_back(animatedJoint);

        // Build the bone matrix for GPU skinning.
        glm::mat4x4 boneTranslate = glm::translate( animatedJoint._Pos );
        glm::mat4x4 boneRotate = glm::toMat4( animatedJoint._Orient );
        glm::mat4x4 boneMatrix = boneTranslate * boneRotate;
        
        skeleton._BoneMatrices.push_back( boneMatrix );
    }

    _Skeletons.push_back(skeleton);

}

void MD5Animation::Update( float fDeltaTime )
{
    if ( _iNumFrames < 1 ) return;

    _fAnimTime += fDeltaTime;

    while ( _fAnimTime > _fAnimDuration ) _fAnimTime -= _fAnimDuration;
    while ( _fAnimTime < 0.0f ) _fAnimTime += _fAnimDuration;

    // Figure out which frame we're on
    float fFramNum = _fAnimTime * (float)_iFramRate;
    int iFrame0 = (int)floorf( fFramNum );
    int iFrame1 = (int)ceilf( fFramNum );
    iFrame0 = iFrame0 % _iNumFrames;
    iFrame1 = iFrame1 % _iNumFrames;

    float fInterpolate = fmodf( _fAnimTime, _fFrameDuration ) / _fFrameDuration;

    InterpolateSkeletons( _AnimatedSkeleton, _Skeletons[iFrame0], _Skeletons[iFrame1], fInterpolate );
}

void MD5Animation::InterpolateSkeletons( FrameSkeleton& finalSkeleton, const FrameSkeleton& skeleton0, const FrameSkeleton& skeleton1, float fInterpolate )
{
    for ( int i = 0; i < _iNumJoints; ++i )
    {
        SkeletonJoint& finalJoint = finalSkeleton._Joints[i];
        glm::mat4x4& finalMatrix = finalSkeleton._BoneMatrices[i];

        const SkeletonJoint& joint0 = skeleton0._Joints[i];
        const SkeletonJoint& joint1 = skeleton1._Joints[i];

        finalJoint._Parent = joint0._Parent;

        finalJoint._Pos = glm::lerp( joint0._Pos, joint1._Pos, fInterpolate );
        finalJoint._Orient = glm::mix( joint0._Orient, joint1._Orient, fInterpolate );

        // Build the bone matrix for GPU skinning.
        finalMatrix = glm::translate( finalJoint._Pos ) * glm::toMat4( finalJoint._Orient );
    }
}

void MD5Animation::Render()
{
    glPointSize( 5.0f );
    glColor3f( 1.0f, 0.0f, 0.0f );

    glPushAttrib( GL_ENABLE_BIT );

    glDisable(GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    const SkeletonJointList& joints = _AnimatedSkeleton._Joints;

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
            const SkeletonJoint& j0 = joints[i];
            if ( j0._Parent != -1 )
            {
                const SkeletonJoint& j1 = joints[j0._Parent];
                glVertex3fv( glm::value_ptr(j0._Pos) );
                glVertex3fv( glm::value_ptr(j1._Pos) );
            }
        }
    }
    glEnd();

    glPopAttrib();
}