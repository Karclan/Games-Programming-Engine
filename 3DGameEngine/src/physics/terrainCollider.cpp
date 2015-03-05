#include "physics\terrainCollider.h"


TerrainCollider::TerrainCollider()
{
	_transform = nullptr;
	_renderer = nullptr;
	_tex = nullptr;

	setDepFlag(ComponentType::TRANSFORM); // requires a transform
	setDepFlag(ComponentType::MODL_REND); // requires a model renderer (to get depth map)
}

void TerrainCollider::linkDependency(SPtr_Component component)
{
	// Switch through type and cast then cache the component
	switch(component->getType())
	{
	case ComponentType::TRANSFORM:
		_transform = std::static_pointer_cast<Transform>(component);
		break;

	case ComponentType::MODL_REND:
		_renderer = std::static_pointer_cast<ModelRenderer>(component);

		break;
	}
}


void TerrainCollider::init()
{
	if(_renderer != nullptr)
		_tex = _renderer->getMaterial()->getTexture();
	else
		_tex = nullptr;
}


bool TerrainCollider::collides(SPtr_Collider other, Collision &collInfo)
{
	if(_tex == nullptr) return false;

	// Get x and z of other collider
	glm::vec3 pos;

	switch(other->getType())
	{
		case ComponentType::SPHERE_COL:
			pos = std::static_pointer_cast<SphereCollider>(other)->getCentre();
			break;
		
		case ComponentType::BOX_COL:
			pos = std::static_pointer_cast<BoxCollider>(other)->getCentre();
			break;

	}
	
	// Get width and height of terrain based on scale of plane?
	// Note plane MUST be 10X10
	glm::vec3 planePos = _transform->getPosition();
	float planeWidth = 10 *_transform->getScale().x;
	float planeBreadth = 10 *_transform->getScale().z;

	float xFrac = pos.x - (planePos.x - (planeWidth * 0.5f));
	if(xFrac < 0 || xFrac >= planeWidth) return false;
	float zFrac = pos.z - (planePos.z - (planeBreadth * 0.5f));
	if(zFrac < 0 || zFrac >= planeBreadth) return false;

	xFrac = xFrac / planeWidth;
	zFrac = zFrac / planeBreadth;
	

	// Note for now shader is hard coded height range of -5 to 5 (this could be dynamically set from here of course)
	int pixelX = (int)(xFrac * _tex->getSize().x);
	int pixelY = _tex->getSize().y - (int)(zFrac * _tex->getSize().y); // invert the y axis
	float height = (float)(_tex->getPixel(pixelX, pixelY).a) / 255;
	height = (height * 10) - 5 + planePos.y;


	if(pos.y < height)
	{
		collInfo.normal = glm::vec3(0, -1, 0);
		collInfo.penDepth = height - pos.y;
		return true;
	}

	return false;
}