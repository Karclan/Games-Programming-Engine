#ifndef MODEL_RENDERER_H
#define MODEL_RENDERER_H

#include "rendering\renderer.h"
#include "rendering\mesh.h"
#include "rendering\material.h"
#include "rendering\animator.h"

#include "rendering\FTInterface.h"

/*! \brief Wrapper for rendering model

	Wraps index of VBO, index of shader, pointer to texture and model matrix
	for a single 3D model in the environment
*/
class ModelRenderer : public Renderer
{
public:
	ModelRenderer();
	ComponentType::Type getType();
	bool isOnePerObject();

	void linkDependency(SPtr_Component component); //!< Override to link needed dependencies, e.g. switch desired types and cache in a variable. Make sure the components have been requested in the dependencyFlags variable.
	

	void render(GLfloat* viewMatrix, GLfloat* projMatrix); //!< Note the args are pointers to the start of the float arrays where the matrix information is stored
	void setMesh(Mesh* mesh);
	void setMaterial(Shader* shader);
	void setMaterial(Shader* shader, Texture2D* textureD, Texture2D* textureS, Texture2D* textureN, Texture2D* textureH, glm::vec2 uvTile, glm::vec3 diffuse, glm::vec3 specular, float specularExponent);

	// Getters for info needed to write to XML / init data
	Mesh * getMesh() { return _mesh; }
	Material * getMaterial() { return &_material; }

private:
	Mesh* _mesh; //!< Pointer to mesh used for rendering

	SPtr_Animator _anim; //!< My lovely animation
	Material _material; //!< Material used for this object. Not a pointer as pretty lightweight, just a pointer to a shader and a pointer to a texture
	
};

//! Define shared pointer to component for easy use by systems (allowing shared responsibility for component as multiple systems may store references to it)
typedef std::shared_ptr<ModelRenderer> SPtr_ModelRend;



#endif