#include "rendering\animation.h"
#include <iostream>


Animation::Animation()
{
	
}

bool Animation::loadAnimation(const std::string& filename)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile( filename, 
        aiProcess_CalcTangentSpace       | 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);


	// If the import failed, report it
	if( !scene)
	{
		std::cout << "Error Loading : " << importer.GetErrorString() << "\n";
		return false;
	}


	// The offset matrices
	glm::mat4 offsets[6]; // hard coded value lol
	int id = 0;
	for(int i = 0; i < scene->mMeshes[0]->mNumBones; ++i)
	{
		if(i ==3 || i == 7) continue;
		std::cout << "Bone " << i << " = " << scene->mMeshes[0]->mBones[i]->mName.C_Str() << "\n";


		aiMatrix4x4 mat = scene->mMeshes[0]->mBones[i]->mOffsetMatrix;
		/*
		glm::mat4 gMat4(mat.a1, mat.a2, mat.a3, mat.a4,
						mat.b1, mat.b2, mat.b3, mat.b4,
						mat.c1, mat.c2, mat.c3, mat.c4,
						mat.d1, mat.d2, mat.d3, mat.d4);


		*/
		glm::mat4 gMat4(mat.a1, mat.b1, mat.c1, mat.d1,
						mat.a2, mat.b2, mat.c2, mat.d2,
						mat.a3, mat.b3, mat.c3, mat.d3,
						mat.a4, mat.b4, mat.c4, mat.d4);


		offsets[id] = gMat4;
		id++;
	}



	if(scene->HasAnimations())
	{
		// We will only the first animation
		aiAnimation* anim = scene->mAnimations[0];

		// First, create empty bone anims
		for(int i = 0; i < anim->mNumChannels; ++i)
		{
			std::cout << "AnimBone " << i << " = " << anim->mChannels[i]->mNodeName.C_Str() << "\n";
			_boneAnims.push_back(BoneAnim());
		}

		// Now fill node anims with data
		for(int i = 0; i < anim->mNumChannels; ++i)
		{
			aiNodeAnim* boneNode = anim->mChannels[i];
			BoneAnim* boneAnim = &_boneAnims[i];


			std::cout << "Translates = " << boneNode->mNumPositionKeys << "\n";
			std::cout << "Rotates = " << boneNode->mNumRotationKeys << "\n";

			for(int j = 0; j < boneNode->mNumRotationKeys; ++j)
			{
				glm::quat rotation;
				rotation.x = boneNode->mRotationKeys[j].mValue.x;
				rotation.y = boneNode->mRotationKeys[j].mValue.y;
				rotation.z = boneNode->mRotationKeys[j].mValue.z;
				rotation.w = boneNode->mRotationKeys[j].mValue.w;

				glm::mat4 mRot = glm::toMat4(rotation);



				glm::vec3 translate;
				translate.x = boneNode->mPositionKeys[j].mValue.x;
				translate.x = boneNode->mPositionKeys[j].mValue.y;
				translate.x = boneNode->mPositionKeys[j].mValue.z;

				glm::mat4 mTrans = glm::translate(translate);

				boneAnim->keys.push_back(mRot * mTrans);
			}
		}

	}


	return true;
}





glm::mat4 Animation::getBoneMatrix(unsigned int bone, unsigned int key)
{
	if(bone >= _boneAnims.size()) return glm::mat4();
	if(key >= _boneAnims[bone].keys.size()) return glm::mat4();

	return _boneAnims[bone].keys[key];

}



/*
bool Animation::LoadAnimation( const std::string& filename )
{

	std::ifstream file;
	file.open(filename);

	if ( !file.is_open())
    {
        std::cerr << "MD5Model::LoadModel: Failed to find file: " << filename << std::endl;
        return false;
    }
	loadAssimpAnim(filename );


    std::string param;
    std::string junk;   // Read junk from the file


    int fileLength = GetFileLength( file );   
    assert( fileLength > 0 );

    _JointInfos.clear();
    _Bounds.clear();
    _BaseFrames.clear();
    _Frames.clear();
    _AnimatedSkeleton.m_Joints.clear();
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
            BuildFrameSkeleton( _Skeletons, _JointInfos, _BaseFrames, frame );

            file >> junk; // Read in the '}' character
            file.ignore(fileLength, '\n' );        
        }

        file >> param;
    }  //while ( !file.eof )

    // Make sure there are enough joints for the animated skeleton.
    _AnimatedSkeleton.m_Joints.assign(_iNumJoints, SkeletonJoint() );
	_AnimatedSkeleton.m_BoneMatrices.assign( _iNumJoints, glm::mat4x4(1.0) );

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

bool Animation::loadAssimpAnim( const std::string& filename )
{

	Assimp::Importer importer;

	animScene = importer.ReadFile( filename, aiProcess_CalcTangentSpace  | 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);
  
	animScene = importer.GetOrphanedScene();
	// If the import failed, report it
	if( !animScene)
	{
		std::cout << "Error Loading : " << importer.GetErrorString() << "\n";
		return nullptr;
	}
	
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

void Animation::BuildFrameSkeleton(FrameSkeletonList& skeletons, const JointInfoList& jointInfos, const BaseFrameList& baseFrames, const Frame& frame )
{
    FrameSkeleton skeleton;

    for ( unsigned int i = 0; i < jointInfos.size(); ++i )
    {
        unsigned int j = 0;

        const JointInfo& jointInfo = jointInfos[i];
        // Start with the base frame position and orientation.
        SkeletonJoint animatedJoint = baseFrames[i];
		
        animatedJoint.m_Parent = jointInfo._parentID;

        if ( jointInfo._flags & 1 ) // Pos.x
        {
            animatedJoint.m_Pos.x = frame._frameData[ jointInfo._startIndex + j++ ];
        }
        if ( jointInfo._flags & 2 ) // Pos.y
        {
            animatedJoint.m_Pos.y = frame._frameData[ jointInfo._startIndex  + j++ ];
        }
        if ( jointInfo._flags & 4 ) // Pos.x
        {
            animatedJoint.m_Pos.z  = frame._frameData[ jointInfo._startIndex  + j++ ];
        }
        if ( jointInfo._flags & 8 ) // Orient.x
        {
            animatedJoint.m_Orient.x = frame._frameData[ jointInfo._startIndex  + j++ ];
        }
        if ( jointInfo._flags & 16 ) // Orient.y
        {
            animatedJoint.m_Orient.y = frame._frameData[ jointInfo._startIndex  + j++ ];
        }
        if ( jointInfo._flags & 32 ) // Orient.z
        {
            animatedJoint.m_Orient.z = frame._frameData[ jointInfo._startIndex  + j++ ];
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

        // Build the bone matrix for GPU skinning.
        glm::mat4x4 boneTranslate = glm::translate( animatedJoint.m_Pos );
        glm::mat4x4 boneRotate = glm::toMat4( animatedJoint.m_Orient );
        glm::mat4x4 boneMatrix = boneTranslate * boneRotate;
        
        skeleton.m_BoneMatrices.push_back( boneMatrix );
    }

    _Skeletons.push_back(skeleton);

}
*/