#ifndef ANIMATION_H
#define ANIMATION_H

#include "core\component.h"
#include "rendering\mesh.h"
#include "rendering\renderer.h"
#include "core\transform.h"

#include <iostream>
#include <fstream>
#include <ctime>

/*! \brief Animation as a component

	Contains all information for MD5 animation
*/

class Animation : public Asset
{
public:
	Animation();


	std::string _filePath;
	struct JointInfo
	{
		std::string _name;
		int _parentID;
		int _flags;
		int _startIndex;
	};
	typedef std::vector<JointInfo> JointInfoList;

	struct Bounds
	{
		glm::vec3 _boundMin;
		glm::vec3 _boundMax;
	};
	typedef std::vector<Bounds> BoundsList;

	struct BaseFrame
	{
		glm::vec3 _position;
		glm::vec3 _orientation;
	};
	typedef std::vector<BaseFrame> BaseFrameList;

	struct Frame
	{
		int _frameNum;
		std::vector<float> _frameData;
	};
	typedef std::vector<Frame> FrameList;
 
    int GetNumJoints() const
    {
        return _iNumJoints;
    }
 
    const JointInfo& GetJointInfo( unsigned int index ) const
    {
        assert( index < _JointInfos.size() );
        return _JointInfos[index];
    }


	bool LoadAnimation( const std::string& filename ); //!< Loads an animation from an MD5 file
	int getNumFrames();
	float getAnimTime();
	float getAnimDuration();
	int getFrameRate();
	float getFrameDuration();



protected:
    JointInfoList       _JointInfos;
    BoundsList          _Bounds;
    BaseFrameList       _BaseFrames;
    FrameList			_Frames;


private:
    int _iMD5Version;
    int _iNumFrames;
    int _iNumJoints;
    int _iFramRate;
    int _iNumAnimatedComponents;

    float _fAnimDuration;
    float _fFrameDuration;
    float _fAnimTime;
};

// Remove the quotes from a string
void RemoveQuotes( std::string& str );

// Get's the size of the file in bytes.
int GetFileLength( std::istream& file );

// Ignore everything else that comes on the line, up to 'length' characters.
void IgnoreLine( std::istream& file, int length );

// Computes the W component of the quaternion based on the X, Y, and Z components.
// This method assumes the quaternion is of unit length.
void ComputeQuatW( glm::quat& quat );


#endif