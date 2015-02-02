#include "rendering\robotRenderer.h"

RobotRenderer::RobotRenderer()
{
	// Set dependency flags so linkDependency is called with requested component
	setDepFlag(ComponentType::TRANSFORM); // requires a transform

	_material.setShader(Assets::getShader("diffuse"));
	_material.setTexture(nullptr);
	generateMesh();

	_animSpeed = 5;
	_playing = true;
}

ComponentType::Type RobotRenderer::getType()
{
	return ComponentType::ROB_REND;
}

bool RobotRenderer::isOnePerObject()
{
	return true;
}

void RobotRenderer::linkDependency(SPtr_Component component)
{
	// Switch through type and cast then cache the component
	switch(component->getType())
	{
	case ComponentType::TRANSFORM:
		_transform = std::static_pointer_cast<Transform>(component);
		break;
	}
}

void RobotRenderer::render(GLfloat* viewMatrix, GLfloat* projMatrix)
{
	// Bind array
	glBindVertexArray(_mesh.getVao());

	// Draw different parts of vertex array object with different matrices
	_material.bind(_transform->getFloatPointer(), viewMatrix, projMatrix);
	glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint))); // Draw Head and torso

	_material.bind(glm::value_ptr(_leftArmBone), viewMatrix, projMatrix);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(72 * sizeof(GLuint))); // Draw left Arm

	_material.bind(glm::value_ptr(_rightArmBone), viewMatrix, projMatrix);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(108 * sizeof(GLuint))); // Draw right Arm

	_material.bind(glm::value_ptr(_leftLegBone), viewMatrix, projMatrix);
	glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, (void*)(144 * sizeof(GLuint))); // Draw left leg and left foot

	_material.bind(glm::value_ptr(_rightLegBone), viewMatrix, projMatrix);
	glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, (void*)(216 * sizeof(GLuint))); // Draw right leg and right foot
}


void RobotRenderer::animate(float t)
{
	if(_playing) // only update animation if playing
	{
		// if leg is moving forward, increase angle, else decrease angle
		for (int side = 0; side < 2; side++)
		{
			// arms
			if (armStates[side] == FORWARD)
				armAngles[side] += 20.0f * _animSpeed * t;
			else
				armAngles[side] -= 20.0f * _animSpeed * t;

			// change state if exceeding angles
			if (armAngles[side] >= 15.0f)
				armStates[side] = BACKWARD;
			else if (armAngles[side] <= -15.0f)
				armStates[side] = FORWARD;

			// legs
			if (legStates[side] == FORWARD)
				legAngles[side] += 20.0f * _animSpeed * t;
			else
				legAngles[side] -= 20.0f * _animSpeed * t;

			// change state if exceeding angles
			if (legAngles[side] >= 15.0f)
				legStates[side] = BACKWARD;
			else if (legAngles[side] <= -15.0f)
				legStates[side] = FORWARD;		
		}
	}

	setMatrices();
}


void RobotRenderer::reset()
{
	armAngles[LEFT] = 0.0;
	armAngles[RIGHT] = 0.0;
	legAngles[LEFT] = 0.0;
	legAngles[RIGHT] = 0.0;

	armStates[LEFT] = FORWARD;
	armStates[RIGHT] = BACKWARD;

	legStates[LEFT] = FORWARD;
	legStates[RIGHT] = BACKWARD;


	setMatrices();
}


void RobotRenderer::setSpeed(float speed)
{
	_animSpeed = speed;
}

void RobotRenderer::play()
{
	_playing = true;
}

void RobotRenderer::stop()
{
	if(_playing)
	{
		reset();
		_playing = false;
	}
}



void RobotRenderer::setMatrices()
{
	// Set matrices
	glm::mat4 rootMat = _transform->getMatrix();
	_leftArmBone = rootMat * glm::translate(glm::vec3(0, 1.5f, 0)) * glm::rotate(glm::radians(armAngles[LEFT]), glm::vec3(1, 0, 0)) * glm::translate(glm::vec3(0, -1.5f, 0));
	_rightArmBone = rootMat * glm::translate(glm::vec3(0, 1.5f, 0)) * glm::rotate(glm::radians(armAngles[RIGHT]), glm::vec3(1, 0, 0)) * glm::translate(glm::vec3(0, -1.5f, 0));

	_leftLegBone = rootMat * glm::translate(glm::vec3(0, -2.5f, 0)) * glm::rotate(glm::radians(legAngles[LEFT]), glm::vec3(1, 0, 0)) * glm::translate(glm::vec3(0, 2.5f, 0));
	_rightLegBone = rootMat * glm::translate(glm::vec3(0, -2.5f, 0)) * glm::rotate(glm::radians(legAngles[RIGHT]), glm::vec3(1, 0, 0)) * glm::translate(glm::vec3(0, 2.5f, 0));

}


void RobotRenderer::generateMesh()
{
	addCube(glm::vec3(0, 3.5f, 0), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(1, 1, 1)); // head
	addCube(glm::vec3(0, 0, 0), glm::vec3(3.0f, 5.0f, 2.0f), glm::vec3(0, 0, 1)); // torso
	addCube(glm::vec3(-2.0f, 0, 0), glm::vec3(1.0f, 4.0f, 1.0f), glm::vec3(1, 0, 0)); // left arm
	addCube(glm::vec3(2.0f, 0, 0), glm::vec3(1.0f, 4.0f, 1.0f), glm::vec3(1, 0, 0)); // right arm
	addCube(glm::vec3(-1.0f, -5.0f, 0), glm::vec3(1.0f, 5.0f, 1.0f), glm::vec3(1, 1, 0)); // left leg
	addCube(glm::vec3(-1.0f, -7.75f, -1.0f), glm::vec3(1.0f, 0.5f, 3.0f), glm::vec3(1, 1, 1)); // left foot
	addCube(glm::vec3(1.0f, -5.0f, 0), glm::vec3(1.0f, 5.0f, 1.0f), glm::vec3(1, 1, 0)); // right leg
	addCube(glm::vec3(1.0f, -7.75f, -1.0f), glm::vec3(1.0f, 0.5f, 3.0f), glm::vec3(1, 1, 1)); // right foot


	// Initialize the mesh and define it via the vectors
	_mesh.generateBuffers();
	_mesh.setVerts(_verts);
	_mesh.setIndices(_indices);
	_mesh.setNormals(_normals);
	_mesh.setUvs(_uvs);
	_mesh.setColours(_colours);
}



void RobotRenderer::addCube(glm::vec3 pos, glm::vec3 scale, glm::vec3 colour)
{
	int startIndex = _verts.size(); // this will be the index of the first vert in this cube

	// verts
	// back face
	_verts.push_back((glm::vec3(-0.5f, 0.5f, 0.5f) * scale) + pos); // tl
	_verts.push_back((glm::vec3(-0.5f, -0.5f, 0.5f) * scale) + pos); // bl
	_verts.push_back((glm::vec3(0.5f, -0.5f, 0.5f) * scale) + pos); // br
	_verts.push_back((glm::vec3(0.5f, 0.5f, 0.5f) * scale) + pos); // tr
	// right face
	_verts.push_back((glm::vec3(0.5f, 0.5f, 0.5f) * scale) + pos); // tl
	_verts.push_back((glm::vec3(0.5f, -0.5f, 0.5f) * scale) + pos); // bl
	_verts.push_back((glm::vec3(0.5f, -0.5f, -0.5f) * scale) + pos); // br
	_verts.push_back((glm::vec3(0.5f, 0.5f, -0.5f) * scale) + pos); // tr
	// front face
	_verts.push_back((glm::vec3(0.5f, 0.5f, -0.5f) * scale) + pos); // tl
	_verts.push_back((glm::vec3(0.5f, -0.5f, -0.5f) * scale) + pos); // bl
	_verts.push_back((glm::vec3(-0.5f, -0.5f, -0.5f) * scale) + pos); // br
	_verts.push_back((glm::vec3(-0.5f, 0.5f, -0.5f) * scale) + pos); // tr
	// left face
	_verts.push_back((glm::vec3(-0.5f, 0.5f, -0.5f) * scale) + pos); // tl
	_verts.push_back((glm::vec3(-0.5f, -0.5f, -0.5f) * scale) + pos); // bl
	_verts.push_back((glm::vec3(-0.5f, -0.5f, 0.5f) * scale) + pos); // br
	_verts.push_back((glm::vec3(-0.5f, 0.5f, 0.5f) * scale) + pos); // tr
	// top face
	_verts.push_back((glm::vec3(-0.5f, 0.5f, -0.5f) * scale) + pos); // tl
	_verts.push_back((glm::vec3(-0.5f, 0.5f, 0.5f) * scale) + pos); // bl
	_verts.push_back((glm::vec3(0.5f, 0.5f, 0.5f) * scale) + pos); // br
	_verts.push_back((glm::vec3(0.5f, 0.5f, -0.5f) * scale) + pos); // tr
	// bottom face
	_verts.push_back((glm::vec3(0.5f, -0.5f, -0.5f) * scale) + pos); // tl
	_verts.push_back((glm::vec3(0.5f, -0.5f, 0.5f) * scale) + pos); // bl
	_verts.push_back((glm::vec3(-0.5f, -0.5f, 0.5f) * scale) + pos); // br
	_verts.push_back((glm::vec3(-0.5f, -0.5f, -0.5f) * scale) + pos); // tr


	// indices
	for(int i = 0; i < 6; i++)//for each face
	{
		unsigned int tl = (i*4) + startIndex; // top left vert as you look at this face is the first vert of the face
		_indices.push_back(0+tl);
		_indices.push_back(1+tl);
		_indices.push_back(2+tl);
		_indices.push_back(0+tl);
		_indices.push_back(2+tl);
		_indices.push_back(3+tl);
	}

	// normals
	for(int i = 0; i < 4; i++) _normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); // normals for 4 verts BACK
	for(int i = 0; i < 4; i++) _normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f)); // normals for 4 verts RIGHT
	for(int i = 0; i < 4; i++) _normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f)); // normals for 4 verts FRONT
	for(int i = 0; i < 4; i++) _normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f)); // normals for 4 verts LEFT
	for(int i = 0; i < 4; i++) _normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // normals for 4 verts TOP
	for(int i = 0; i < 4; i++) _normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f)); // normals for 4 verts BOTTOM

	// uvs
	for(int i = 0; i < 6; i++)//for each face
	{
		_uvs.push_back(glm::vec2(0.0, 1.0)); // tl
		_uvs.push_back(glm::vec2(0.0, 0.0)); // bl
		_uvs.push_back(glm::vec2(1.0, 0.0)); // br
		_uvs.push_back(glm::vec2(1.0, 1.0)); // tr
	}

	// colours
	for(int i = 0; i < 24; i++) _colours.push_back(colour); // 24 verts per cube

	
}