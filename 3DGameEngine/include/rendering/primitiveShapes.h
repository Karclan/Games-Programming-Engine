#ifndef PRIMITIVE_SHAPES_H
#define PRIMITIVE_SHAPES_H

#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "rendering\mesh.h"


/*! \brief A set of primitive meshes for use in game

	
*/
class PrimitiveShapes
{
public:
	enum Type { TRIANGLE, CUBE, SPHERE, NUM_SHAPES };
	
	PrimitiveShapes();
	
	void loadShapes();
	Mesh* getMesh(Type type);







private:
	void loadTriangle();
	void loadCube();
	void loadSphere();

	Mesh _meshes[NUM_SHAPES];



	

};


#endif