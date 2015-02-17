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

class Animation : public Component
{
public:
	Animation();

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

	 // A Skeleton joint is a joint of the skeleton per frame
    struct SkeletonJoint
    {
        SkeletonJoint()
            : m_Parent(-1)
            , m_Pos(0)
        {}
 
        SkeletonJoint( const BaseFrame& copy )
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
        SkeletonJointList   m_Joints;
    };
    typedef std::vector<FrameSkeleton> FrameSkeletonList;
 
    const FrameSkeleton& GetSkeleton() const
    {
        return _AnimatedSkeleton;
    }
 
    int GetNumJoints() const
    {
        return _iNumJoints;
    }
 
    const JointInfo& GetJointInfo( unsigned int index ) const
    {
        assert( index < _JointInfos.size() );
        return _JointInfos[index];
    }


	ComponentType::Type getType(); //!< Required implementation. Return type of component
	bool isOnePerObject(); //!< Required implementation. Return true if you can only have one of these per object
	bool LoadAnimation( const std::string& filename ); //!< Loads an animation from an MD5 file
	void BuildFrameSkeleton( FrameSkeletonList& skeletons, const JointInfoList& jointInfos, const BaseFrameList& baseFrames, const Frame& frame );

protected:

    JointInfoList       _JointInfos;
    BoundsList          _Bounds;
    BaseFrameList       _BaseFrames;
    FrameList			_Frames;

	FrameSkeletonList   _Skeletons;    // All the skeletons for all the frames
    FrameSkeleton       _AnimatedSkeleton;

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

//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<Animation> SPtr_Animation;

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