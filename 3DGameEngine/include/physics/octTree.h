#ifndef OCT_TREE_H
#define OCT_TREE_H

#include <vector>

#include "physics\collider.h"
#include "physics\physicsBody.h"

class OctTreeNode
{
public:
	OctTreeNode(){}

	//void addRegCollider(SPtr_Collider collider) { _regColliders.push_back(collider); }
	//void addPhysCollider(SPtr_Collider collider) { _physColliders.push_back(collider); }
	//void clear() { _regColliders.clear(); _physColliders.clear; }

	std::vector<SPtr_Collider> regColliders; //!< Regular colliders, non-static but also non-reactionary
	std::vector<SPtr_Collider> physColliders; //!< Physics body colliders, will move, react and require info about collisions
	void clear() { regColliders.clear(); physColliders.clear(); }


private:
	//std::vector<SPtr_Collider> _regColliders; //!< Regular colliders, non-static but also non-reactionary
	//std::vector<SPtr_Collider> _physColliders; //!< Physics body colliders, will move, react and require info about collisions
	
};




#endif