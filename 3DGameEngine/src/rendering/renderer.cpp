#include "rendering\renderer.h"

Renderer::Renderer()
{
	_transform = nullptr;
}


void Renderer::setTransform(SPtr_Transform transform)
{ 
	_transform = transform; 
} 
