#include "rendering\animation.h"
#include <iostream>


Animation::Animation()
{
	
}

bool Animation::LoadAnimation( const std::string& filename )
{

	std::ifstream file;
	file.open(filename);

	if ( !file.is_open())
    {
        std::cerr << "MD5Model::LoadModel: Failed to find file: " << filename << std::endl;
        return false;
    }



    std::string param;
    std::string junk;   // Read junk from the file


    int fileLength = GetFileLength( file );   
    assert( fileLength > 0 );

    _JointInfos.clear();
    _Bounds.clear();
    _BaseFrames.clear();
    _Frames.clear();
    //_AnimatedSkeleton.m_Joints.clear();
    _iNumFrames = 0;

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
                file >> joint._name >> joint._parentID >> joint._flags >> joint._startIndex;
                RemoveQuotes( joint._name );

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
                Bounds bound;
                file >> junk; // read in the '(' character
                file >> bound._boundMin.x >> bound._boundMin.y >> bound._boundMin.z;
                file >> junk >> junk; // read in the ')' and '(' characters.
                file >> bound._boundMax.x >> bound._boundMax.y >> bound._boundMax.z;
                
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
                file >> baseFrame._position.x >> baseFrame._position.y >> baseFrame._position.z;
                file >> junk >> junk;
                file >> baseFrame._orientation.x >> baseFrame._orientation.y >> baseFrame._orientation.z;
                file.ignore( fileLength, '\n' );

                _BaseFrames.push_back(baseFrame);
            }
            file >> junk; // read in the '}' character
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "frame" )
        {
            Frame frame;
            file >> frame._frameNum >> junk; // Read in the '{' character
            file.ignore(fileLength, '\n' );

            for ( int i = 0; i < _iNumAnimatedComponents; ++i )
            {
                float frameData;
                file >> frameData;
                frame._frameData.push_back(frameData);
            }

            _Frames.push_back(frame);

            // Build a skeleton for this frame
           // BuildFrameSkeleton( _Skeletons, _JointInfos, _BaseFrames, frame );

            file >> junk; // Read in the '}' character
            file.ignore(fileLength, '\n' );        
        }

        file >> param;
    }  //while ( !file.eof )

    // Make sure there are enough joints for the animated skeleton.
  //  _AnimatedSkeleton.m_Joints.assign(_iNumJoints, SkeletonJoint() );

    _fFrameDuration = 1.0f / (float)_iFramRate;
    _fAnimDuration = ( _fFrameDuration * (float)_iNumFrames );
    _fAnimTime = 0.0f;

    assert( _JointInfos.size() == _iNumJoints );
    assert( _Bounds.size() == _iNumFrames );
    assert( _BaseFrames.size() == _iNumJoints );
    assert( _Frames.size() == _iNumFrames );
   // assert( _Skeletons.size() == _iNumFrames );

    return true;
}

int Animation::getNumFrames()
{
	return _iNumFrames;
}

float Animation::getAnimTime()
{
	return _fAnimTime;
}

float Animation::getAnimDuration()
{
	return _fAnimDuration;
}

int Animation::getFrameRate()
{
	return _iFramRate;
}

float Animation::getFrameDuration()
{
	return _fFrameDuration;
}
// Remove the quotes from a string
void RemoveQuotes( std::string& str )
{
    size_t n;
    while ( ( n = str.find('\"') ) != std::string::npos ) str.erase(n,1);
}

// Get's the size of the file in bytes.
int GetFileLength( std::istream& file )
{
    int pos = file.tellg();
    file.seekg(0, std::ios::end );
    int length = file.tellg();
    // Restore the position of the get pointer
    file.seekg(pos);

    return length;
}

void IgnoreLine( std::istream& file, int length )
{
    file.ignore( length, '\n' );
}

// Computes the W component of the quaternion based on the X, Y, and Z components.
// This method assumes the quaternion is of unit length.
void ComputeQuatW( glm::quat& quat )
{
    float t = 1.0f - ( quat.x * quat.x ) - ( quat.y * quat.y ) - ( quat.z * quat.z );
    if ( t < 0.0f )
    {
        quat.w = 0.0f;
    }
    else
    {
        quat.w = -sqrtf(t);
    }
}
