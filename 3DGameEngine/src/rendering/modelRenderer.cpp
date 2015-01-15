#include "rendering\modelRenderer.h"

ModelRenderer::ModelRenderer()
{
	_mesh = nullptr;
}

void ModelRenderer::render(GLfloat* viewMatrix, GLfloat* projMatrix)
{
	if(_mesh == nullptr) return; // Can't render without mesh

	_material.bind(_transform->getFloatPointer(), viewMatrix, projMatrix);
	glBindVertexArray(_mesh->getVao());
	glDrawElements(GL_TRIANGLES, _mesh->numIndices(), GL_UNSIGNED_INT, (void*)0);
}

ComponentType::Type ModelRenderer::getType()
{
	return ComponentType::MODL_REND;
}

bool ModelRenderer::isOnePerObject()
{
	return false;
}

void ModelRenderer::setMesh(Mesh* mesh)
{
	_mesh = mesh;
}


void ModelRenderer::setMaterial(Shader* shader)
{
	_material.setShader(shader);
	_material.setTexture(nullptr); // resets the texture when you set material without one
	_material.setUVTiling(glm::vec2(1, 1)); // default value
}

void ModelRenderer::setMaterial(Shader* shader, Texture2D* texture, glm::vec2 uvTile)
{
	_material.setShader(shader);
	_material.setTexture(texture);
	_material.setUVTiling(uvTile);
}