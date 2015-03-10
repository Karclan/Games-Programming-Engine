#include "rendering\modelRenderer.h"

ModelRenderer::ModelRenderer()
{
	// Set dependency flags so linkDependency is called with requested component
	setDepFlag(ComponentType::TRANSFORM); // requires a transform

	_mesh = nullptr;
}

void ModelRenderer::render(GLfloat* viewMatrix, GLfloat* projMatrix)
{
	if(_mesh == nullptr) return; // Can't render without mesh
	_material.bind(_transform->getMatrix(), viewMatrix, projMatrix);
	glBindVertexArray(_mesh->getVao());
	glDrawElements(GL_TRIANGLES, _mesh->numIndices(), GL_UNSIGNED_INT, (void*)0);
	_material.unbind();
}

ComponentType::Type ModelRenderer::getType()
{
	return ComponentType::MODL_REND;
}

bool ModelRenderer::isOnePerObject()
{
	return false;
}

void ModelRenderer::linkDependency(SPtr_Component component)
{
	// Switch through type and cast then cache the component
	switch(component->getType())
	{
	case ComponentType::TRANSFORM:
		_transform = std::static_pointer_cast<Transform>(component);
		break;
	}
}

void ModelRenderer::setMesh(Mesh* mesh)
{
	_mesh = mesh;
}


void ModelRenderer::setMaterial(Shader* shader)
{
	_material.setShader(shader);
	_material.setTexture(nullptr,Material::DIFFUSE); // resets the texture when you set material without one
	_material.setTexture(nullptr,Material::SPECULAR); // resets the texture when you set material without one
	_material.setTexture(nullptr,Material::NORMAL); // resets the texture when you set material without one
	_material.setTexture(nullptr,Material::HEIGHT); // resets the texture when you set material without one
	_material.setUVTiling(glm::vec2(1, 1)); // default value
}

void ModelRenderer::setMaterial(Shader* shader, Texture2D* textureD, Texture2D* textureS, Texture2D* textureN, Texture2D* textureH, glm::vec2 uvTile, glm::vec3 diffuse, glm::vec3 specular, float specularExponent)
{

	_material.setShader		(shader);
	_material.setTexture	(textureD,Material::DIFFUSE);
	_material.setTexture	(textureS,Material::SPECULAR);
	_material.setTexture	(textureN,Material::NORMAL);
	_material.setTexture	(textureH,Material::HEIGHT);
	_material.setUVTiling	(uvTile);
	_material.setDiffuse	(diffuse);
	_material.setSpecular	(specular);
	_material.setSpecularExp(specularExponent);
}