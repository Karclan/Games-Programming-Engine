#include "rendering\primitiveShapes.h"

PrimitiveShapes::PrimitiveShapes()
{
}

void PrimitiveShapes::loadShapes()
{
	loadTriangle();
	loadCube();
	loadSphere();

	for(int i = 0; i < NUM_SHAPES; ++i)
	{
		_meshes[i].setFilePath("");
		_meshes[i].setPrimID(i);
	}
}

Mesh* PrimitiveShapes::getMesh(Type type)
{
	return &_meshes[type];
}

void PrimitiveShapes::loadTriangle()
{
	// verts
	std::vector<glm::vec3> verts;
	verts.push_back(glm::vec3(0.0f, 0.5f, 0.0f));
	verts.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
	verts.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
	
	// indices
	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	// normals
	std::vector<glm::vec3> normals;
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

	// uvs
	std::vector<glm::vec2> uvs;
	uvs.push_back(glm::vec2(0.5f, 1.0f));
	uvs.push_back(glm::vec2(0.0f, 0.0f));
	uvs.push_back(glm::vec2(1.0f, 0.0f));

	// colours
	std::vector<glm::vec3> colours;
	for(int i = 0; i < verts.size(); i++) colours.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	// Initialize the triangle mesh and define it via the above vectors
	_meshes[TRIANGLE].generateBuffers();
	_meshes[TRIANGLE].setVerts(verts);
	_meshes[TRIANGLE].setIndices(indices);
	_meshes[TRIANGLE].setNormals(normals);
	_meshes[TRIANGLE].setUvs(uvs);
	_meshes[TRIANGLE].setColours(colours);
}


void PrimitiveShapes::loadCube()
{
	// note "back" refers to the face that faces camera as it is "behind" in relation to viewer
	// "forwards" refers to going into the screen, which is negative in OpenGL, so the "front" face is the one we can't see
	// when we're looking at it from +5 in z and it is at 0, 0, 0 with no rotation

	// verts
	std::vector<glm::vec3> verts;
	
	// back face
	verts.push_back(glm::vec3(-0.5f, 0.5f, 0.5f)); // tl
	verts.push_back(glm::vec3(-0.5f, -0.5f, 0.5f)); // bl
	verts.push_back(glm::vec3(0.5f, -0.5f, 0.5f)); // br
	verts.push_back(glm::vec3(0.5f, 0.5f, 0.5f)); // tr
	// right face
	verts.push_back(glm::vec3(0.5f, 0.5f, 0.5f)); // tl
	verts.push_back(glm::vec3(0.5f, -0.5f, 0.5f)); // bl
	verts.push_back(glm::vec3(0.5f, -0.5f, -0.5f)); // br
	verts.push_back(glm::vec3(0.5f, 0.5f, -0.5f)); // tr
	// front face
	verts.push_back(glm::vec3(0.5f, 0.5f, -0.5f)); // tl
	verts.push_back(glm::vec3(0.5f, -0.5f, -0.5f)); // bl
	verts.push_back(glm::vec3(-0.5f, -0.5f, -0.5f)); // br
	verts.push_back(glm::vec3(-0.5f, 0.5f, -0.5f)); // tr
	// left face
	verts.push_back(glm::vec3(-0.5f, 0.5f, -0.5f)); // tl
	verts.push_back(glm::vec3(-0.5f, -0.5f, -0.5f)); // bl
	verts.push_back(glm::vec3(-0.5f, -0.5f, 0.5f)); // br
	verts.push_back(glm::vec3(-0.5f, 0.5f, 0.5f)); // tr
	// top face
	verts.push_back(glm::vec3(-0.5f, 0.5f, -0.5f)); // tl
	verts.push_back(glm::vec3(-0.5f, 0.5f, 0.5f)); // bl
	verts.push_back(glm::vec3(0.5f, 0.5f, 0.5f)); // br
	verts.push_back(glm::vec3(0.5f, 0.5f, -0.5f)); // tr
	// bottom face
	verts.push_back(glm::vec3(0.5f, -0.5f, -0.5f)); // tl
	verts.push_back(glm::vec3(0.5f, -0.5f, 0.5f)); // bl
	verts.push_back(glm::vec3(-0.5f, -0.5f, 0.5f)); // br
	verts.push_back(glm::vec3(-0.5f, -0.5f, -0.5f)); // tr


	// indices
	std::vector<unsigned int> indices;
	for(int i = 0; i < 6; i++)//for each face
	{
		unsigned int tl = i*4; // top left vert as you look at this face is the first vert of the face
		indices.push_back(0+tl);
		indices.push_back(1+tl);
		indices.push_back(2+tl);
		indices.push_back(0+tl);
		indices.push_back(2+tl);
		indices.push_back(3+tl);
	}

	// normals
	std::vector<glm::vec3> normals;
	for(int i = 0; i < 4; i++) normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); // normals for 4 verts BACK
	for(int i = 0; i < 4; i++) normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f)); // normals for 4 verts RIGHT
	for(int i = 0; i < 4; i++) normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f)); // normals for 4 verts FRONT
	for(int i = 0; i < 4; i++) normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f)); // normals for 4 verts LEFT
	for(int i = 0; i < 4; i++) normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // normals for 4 verts TOP
	for(int i = 0; i < 4; i++) normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f)); // normals for 4 verts BOTTOM

	// uvs
	std::vector<glm::vec2> uvs;
	for(int i = 0; i < 6; i++)//for each face
	{
		uvs.push_back(glm::vec2(0.0, 1.0)); // tl
		uvs.push_back(glm::vec2(0.0, 0.0)); // bl
		uvs.push_back(glm::vec2(1.0, 0.0)); // br
		uvs.push_back(glm::vec2(1.0, 1.0)); // tr
	}

	// colours
	std::vector<glm::vec3> colours;
	for(int i = 0; i < verts.size(); i++) colours.push_back(glm::vec3(1.0f, 1.0f, 1.0f));


	// Initialize the triangle mesh and define it via the above vectors
	_meshes[CUBE].generateBuffers();
	_meshes[CUBE].setVerts(verts);
	_meshes[CUBE].setIndices(indices);
	_meshes[CUBE].setNormals(normals);
	_meshes[CUBE].setUvs(uvs);
	_meshes[CUBE].setColours(colours);
}




void PrimitiveShapes::loadSphere()
{
	int rings = 10;
	int divs = 10;
	float radius = 0.5f;

	std::vector<glm::vec3> verts;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> colours;


	float const R = 1./(float)(rings-1);
    float const D = 1./(float)(divs-1);

    for(int r = 0; r < rings; r++)
	{
		for(int d = 0; d < divs; d++)
		{
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * d * D) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * d * D) * sin( M_PI * r * R );

			
			verts.push_back(glm::vec3(x * radius, y * radius, z * radius));
			normals.push_back(glm::vec3(x, y, z));
			uvs.push_back(glm::vec2(d*D, r*R));
		}
    }

    for(int r = 0; r < rings; r++)
	{
		for(int d = 0; d < divs; d++)
		{
			int tl = r * divs + d; // top left
            int tr = r * divs + (d+1); // top right
            int br = (r+1) * divs + (d+1); // bottom right
            int bl =(r+1) * divs + d; // bottom left


			indices.push_back(tl);
			indices.push_back(bl);
			indices.push_back(br);
			indices.push_back(tl);
			indices.push_back(br);
			indices.push_back(tr);

		}
    }

	// colours
	for(int i = 0; i < verts.size(); i++) colours.push_back(glm::vec3(1.0f, 1.0f, 1.0f));



	// Initialize the sphere mesh and define it via the above vectors
	_meshes[SPHERE].generateBuffers();
	_meshes[SPHERE].setVerts(verts);
	_meshes[SPHERE].setIndices(indices);
	_meshes[SPHERE].setNormals(normals);
	_meshes[SPHERE].setUvs(uvs);
	_meshes[SPHERE].setColours(colours);
	
}