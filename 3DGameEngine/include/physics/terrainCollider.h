#ifndef TERRAIN_COLLIDER_H
#define TERRAIN_COLLIDER_H

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "core\component.h"
#include "core\transform.h"
#include "rendering\modelRenderer.h"
#include "physics\physicsBody.h"
#include "physics\collider.h"

class TerrainCollider : public Component
{
public:
	TerrainCollider();

	ComponentType::Type getType() { return ComponentType::TERRAIN_COL; }
	bool isOnePerObject() { return true; }
	void linkDependency(SPtr_Component component); //!< Override to link needed dependencies, e.g. switch desired types and cache in a variable. Make sure the components have been requested in the dependencyFlags variable.
	void init();

	bool collides(SPtr_Collider other, Collision &collInfo);


protected:
	SPtr_Transform _transform; //!< Pointer to transform
	SPtr_ModelRend _renderer;
	Texture2D* _tex; //!< Pointer to the texture2D so we can use image data to determine height
};

typedef std::shared_ptr<TerrainCollider> SPtr_TerrainCol;

#endif