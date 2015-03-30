#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "core\component.h"
#include "rendering\shader.h"
#include "core\transform.h"
#include "rendering\animation.h"
#include "rendering/mesh.h"

#include <iostream>
#include <fstream>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <assimp\Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/matrix4x4.h>

class Animator : public Component
{
public:
	Animator();

	

	ComponentType::Type getType(); //!< Required implementation. Return type of component
	bool isOnePerObject(); //!< Required implementation. Return true if you can only have one of these per object
	void UpdateAnim( float fDeltaTime ); //!< update function for the animation
	void Update( float fDeltaTime );	//!< update function for animator
	void bind(Shader* shader);			//!< binds the transformation matrix to the shader

	void InterpolateSkeletons( Animation::FrameSkeleton& finalSkeleton, const Animation::FrameSkeleton& skeleton0, const Animation::FrameSkeleton& skeleton1, float fInterpolate );
	void setBones(std::vector<GLint [4]> &boneIds, std::vector<GLfloat [4]> &boneWeights); //!< Fill bone data via vector

	
	void setMesh(Mesh* m); //!< sets the mesh being animated
	void checkValidity(Mesh m, Animation a); //!<checks the validity between the mesh and animation (if same number of joints)
	

	void setAnimation(Animation* anim) 
	{ 
		if(anim != nullptr)
		{
			_animation = anim; 
		}
	}
	Animation* getAnimation(){return _animation;}
protected:



private:
	Animation* _animation;
	Mesh* _mesh;
	typedef std::vector<glm::mat4x4> MatrixList;
	std::vector<glm::mat4> finalTransform;
	float _fAnimTime;
	MatrixList _AnimatedBones;
	std::vector<glm::mat4> Transforms; 
	glm::mat4 dummy;
};



//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<Animator> SPtr_Animator;

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