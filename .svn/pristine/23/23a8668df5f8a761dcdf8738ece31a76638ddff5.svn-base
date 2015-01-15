#ifndef PRIMITIVE_SHAPES_H
#define PRIMITIVE_SHAPES_H

#include <vector>

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "rendering\mesh.h"


/*! \brief A set of primitive meshes for use in game

	
*/
class PrimitiveShapes
{
public:
	enum Type { TRIANGLE, CUBE, NUM_SHAPES };
	
	PrimitiveShapes();
	
	void loadShapes();
	Mesh* getMesh(Type type);







private:
	void loadTriangle();
	void loadCube();

	Mesh _meshes[NUM_SHAPES];




};


#endif