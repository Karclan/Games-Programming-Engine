#pragma once;
#include <assert.h>

#include "animation/Helpers.h"

class animMD5
{
public:
    animMD5();
    virtual ~animMD5();

    // Load an animation from the animation file
    bool LoadAnimation( const std::string& filename );
    // Update this animation's joint set.
    void Update( float fDeltaTime );
    // Draw the animated skeleton
    void Render();

    // The JointInfo stores the information necessary to build the 
    // skeletons for each frame
    struct JointInfo
    {
        std::string _Name;
        int         _ParentID;
        int         _Flags;
        int         _StartIndex;
    };
    typedef std::vector<JointInfo> JointInfoList;

    struct Bound
    {
        glm::vec3   _Min;
        glm::vec3   _Max;
    };
    typedef std::vector<Bound> BoundList;

    struct BaseFrame
    {
        glm::vec3   _Pos;
        glm::quat   _Orient;	//without a pointer it is not allowed
    };
    typedef std::vector<BaseFrame> BaseFrameList;

    struct FrameData
    {
        int _iFrameID;
        std::vector<float> _FrameData;
    };
    typedef std::vector<FrameData> FrameDataList;

    // A Skeleton joint is a joint of the skeleton per frame
    struct SkeletonJoint
    {
        SkeletonJoint()
            : _Parent(-1)
            , _Pos(0)
        {}

        SkeletonJoint( const BaseFrame& copy )
            : _Pos( copy._Pos )
            , _Orient( copy._Orient )
        {}

        int         _Parent;
        glm::vec3   _Pos;
        glm::quat   _Orient;	//without a pointer it is not allowed
    };
    typedef std::vector<SkeletonJoint> SkeletonJointList;
    typedef std::vector<glm::mat4x4> SkeletonMatrixList;

    // A frame skeleton stores the joints of the skeleton for a single frame.
    struct FrameSkeleton
    {
        SkeletonMatrixList  _BoneMatrices;
        SkeletonJointList   _Joints;
    };
    typedef std::vector<FrameSkeleton> FrameSkeletonList;

    const FrameSkeleton& GetSkeleton() //const
    {
        return _AnimatedSkeleton;
    }

    const SkeletonMatrixList& GetSkeletonMatrixList() const
    {
        return _AnimatedSkeleton._BoneMatrices;
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

protected:

    JointInfoList       _JointInfos;
    BoundList           _Bounds;
    BaseFrameList       _BaseFrames;
    FrameDataList       _Frames;
    FrameSkeletonList   _Skeletons;    // All the skeletons for all the frames

    FrameSkeleton       _AnimatedSkeleton;

    // Build the frame skeleton for a particular frame
    void BuildFrameSkeleton( FrameSkeletonList& skeletons, const JointInfoList& jointInfo, const BaseFrameList& baseFrames, const FrameData& frameData );
    void InterpolateSkeletons( FrameSkeleton& finalSkeleton, const FrameSkeleton& skeleton0, const FrameSkeleton& skeleton1, float fInterpolate );


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