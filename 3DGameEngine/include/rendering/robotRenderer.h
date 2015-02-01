#ifndef ROBOT_RENDERER_H
#define ROBOT_RENDERER_H

#include "rendering\renderer.h"
#include "rendering\material.h"
#include "rendering\mesh.h"
#include "core\assets.h"

/*! \brief Temporary renderer for robot

	As a generic animation system is beyond the scope of the initial submission this renderer
	has been created as a hard-coded way of getting the example robot into the project.
*/
class RobotRenderer : public Renderer
{
public:
	RobotRenderer();
	ComponentType::Type getType();
	bool isOnePerObject();

	void linkDependency(SPtr_Component component); //!< Override to link needed dependencies, e.g. switch desired types and cache in a variable. Make sure the components have been requested in the dependencyFlags variable.
	
	void reset();
	void render(GLfloat* viewMatrix, GLfloat* projMatrix); //!< Note the args are pointers to the start of the float arrays where the matrix information is stored
	void animate(float t);
	
	// Animation Playback functions
	void setSpeed(float speed);
	void play();
	void stop();

private:
	Material _material; //!< Material used for this object. Not a pointer as pretty lightweight, just a pointer to a shader and a pointer to a texture
	Mesh _mesh; //!< Mesh for the robot. Not a pointer as there will never be more than 1 robot and this is temporary (not final animation solution)

	// Vectors for storing info in as it is generated
	std::vector<glm::vec3> _verts;
	std::vector<unsigned int> _indices;
	std::vector<glm::vec3> _normals;
	std::vector<glm::vec2> _uvs;
	std::vector<glm::vec3> _colours;

	// Matrices for animated limbs
	glm::mat4 _leftArmBone;
	glm::mat4 _rightArmBone;
	glm::mat4 _leftLegBone;
	glm::mat4 _rightLegBone;

	enum BoneState { BACKWARD, FORWARD };
	enum LimbSide { RIGHT = 0, LEFT = 1 };
	int legStates[2];	
	int armStates[2];

	float legAngles[2];
	float armAngles[2];

	float _animSpeed;
	bool _playing;


	// Private Functions
	void generateMesh(); //!< Fill the mesh with correct verts etc
	void addCube(glm::vec3 pos, glm::vec3 scale, glm::vec3 colour); //!< Add a cube to the data vectors
	void setMatrices(); //!< This will calculate correct matrices for bones
};



#endif