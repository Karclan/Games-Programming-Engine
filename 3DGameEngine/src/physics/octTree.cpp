#include "physics\octTree.h"

OctTreeNode::OctTreeNode(AABB bounds, unsigned int depth, unsigned int maxCols, unsigned int maxDepth)
{
	_bounds = bounds;
	_depth = depth;
	_maxCols = maxCols;
	_maxCols = maxDepth;
	_centre = _bounds.min + ((_bounds.max - _bounds.min) * 0.5f);

	for(int i = 0; i < 8; ++i)
	{
		_children[i] = nullptr;
	}
}

void OctTreeNode::insert(SPtr_Collider collider)
{
	// If any child is null then this node is a leaf node
	if(_children[0] == nullptr) // IF LEAF...
	{
		// Add to colliders
		_colliders.push_back(collider);

		// Split if at not at max depth and too many colliders
		if(_depth < _maxDepth && _colliders.size() > _maxCols)
		{
			// Create child nodes
			createChildNodes();

			// After division, call insert on all colliders to re-insert them to this node (thus inserting them into correct child node)
			for(int i = 0; i < _colliders.size(); ++i)
			{
				insert(_colliders[i]);
			}
			
			_colliders.clear();
		}
	}
	else // IS NOT LEAF ...
	{
		// determine which child node(s) this collider falls into and insert it
		AABB colBounds = collider->getBounds();

		if(colBounds.max.x > _bounds.min.x && colBounds.min.x < _centre.x) // falls into left
		{
			if(colBounds.max.y > _bounds.min.y && colBounds.min.y < _centre.y) // falls into bottom
			{
				if(colBounds.max.z > _bounds.min.z && colBounds.min.z < _centre.z) // falls into back
				{
					_children[BLB]->insert(collider);// Bottom Left Back
				}

				if(colBounds.max.z > _centre.z && colBounds.min.z < _bounds.max.z) // falls into front
				{
					_children[BLF]->insert(collider);// Bottom Left Front
				}
			}

			if(colBounds.max.y > _centre.y && colBounds.min.y < _bounds.max.y) // falls into top
			{
				if(colBounds.max.z > _bounds.min.z && colBounds.min.z < _centre.z) // falls into back
				{
					_children[TLB]->insert(collider);// Top Left Back
				}

				if(colBounds.max.z > _centre.z && colBounds.min.z < _bounds.max.z) // falls into front
				{
					_children[TLF]->insert(collider);// Top Left Front
				}
			}
		}

		if(colBounds.max.x > _centre.x && colBounds.min.x < _bounds.max.x) // falls into right
		{
			if(colBounds.max.y > _bounds.min.y && colBounds.min.y < _centre.y) // falls into bottom
			{
				if(colBounds.max.z > _bounds.min.z && colBounds.min.z < _centre.z) // falls into back
				{
					_children[BRB]->insert(collider);// Bottom Right Back
				}

				if(colBounds.max.z > _centre.z && colBounds.min.z < _bounds.max.z) // falls into front
				{
					_children[BRF]->insert(collider);// Bottom Right Front
				}
			}

			if(colBounds.max.y > _centre.y && colBounds.min.y < _bounds.max.y) // falls into top
			{
				if(colBounds.max.z > _bounds.min.z && colBounds.min.z < _centre.z) // falls into back
				{
					_children[TRB]->insert(collider);// Top Right Back
				}

				if(colBounds.max.z > _centre.z && colBounds.min.z < _bounds.max.z) // falls into front
				{
					_children[TRF]->insert(collider);// Top Right Front
				}
			}
		}
	}
}



// Generate collision matrix. Note this is where layers could also be calculated
void OctTreeNode::addToColMatrix(std::map<SPtr_Collider, std::set<SPtr_Collider>> &matrix)
{
	// if leaf node, add colliders to matrix
	if(_children[0] == nullptr) // IF LEAF...
	{
		SPtr_Collider* sleepingColliders = new SPtr_Collider[_colliders.size()];
		int s = 0;

		for(int i = 0; i < _colliders.size(); ++i)
		{
			// Skip inactive colliders
			if(!_colliders[i]->isActive()) continue;

			// Skip sleeping colliders but store in array so other things can potentially collide with them
			if(!_colliders[i]->getPhysicsBody()->isAwake() && _colliders[i]->isActive())
			{
				sleepingColliders[s] = _colliders[i];
				s++;
				continue;
			}

			// Add all colliders after this one
			for(int j = i+1; j < _colliders.size(); ++j)
			{
				if(!_colliders[j]->isActive()) continue;
				matrix[_colliders[i]].insert(_colliders[j]);
			}

			// Add any sleeping colliders that occured before this one
			for(int k = 0; k < s; ++k)
			{
				matrix[_colliders[i]].insert(sleepingColliders[k]);
			}
		}

		delete[] sleepingColliders;

	}
	else // not leaf, call on children
	{
		for(int i = 0; i < 8; ++i)
		{
			_children[i]->addToColMatrix(matrix);
		}
	}
}



void OctTreeNode::addToColliders(const AABB &aabb, std::set<SPtr_Collider> &colliders)
{
	// If leaf
	if(_children[0] == nullptr)
	{
		for(int i = 0 ; i < _colliders.size(); ++i)
		{
			colliders.insert(_colliders[i]);
		}
		return;
	}

	// Not leaf
	if(aabb.max.x > _bounds.min.x && aabb.min.x < _centre.x) // falls into left
	{
		if(aabb.max.y > _bounds.min.y && aabb.min.y < _centre.y) // falls into bottom
		{
			if(aabb.max.z > _bounds.min.z && aabb.min.z < _centre.z) // falls into back
			{
				_children[BLB]->addToColliders(aabb, colliders);// Bottom Left Back
			}

			if(aabb.max.z > _centre.z && aabb.min.z < _bounds.max.z) // falls into front
			{
				_children[BLF]->addToColliders(aabb, colliders);// Bottom Left Front
			}
		}

		if(aabb.max.y > _centre.y && aabb.min.y < _bounds.max.y) // falls into top
		{
			if(aabb.max.z > _bounds.min.z && aabb.min.z < _centre.z) // falls into back
			{
				_children[TLB]->addToColliders(aabb, colliders);// Top Left Back
			}

			if(aabb.max.z > _centre.z && aabb.min.z < _bounds.max.z) // falls into front
			{
				_children[TLF]->addToColliders(aabb, colliders);// Top Left Front
			}
		}
	}

	if(aabb.max.x > _centre.x && aabb.min.x < _bounds.max.x) // falls into right
	{
		if(aabb.max.y > _bounds.min.y && aabb.min.y < _centre.y) // falls into bottom
		{
			if(aabb.max.z > _bounds.min.z && aabb.min.z < _centre.z) // falls into back
			{
				_children[BRB]->addToColliders(aabb, colliders);// Bottom Right Back
			}

			if(aabb.max.z > _centre.z && aabb.min.z < _bounds.max.z) // falls into front
			{
				_children[BRF]->addToColliders(aabb, colliders);// Bottom Right Front
			}
		}

		if(aabb.max.y > _centre.y && aabb.min.y < _bounds.max.y) // falls into top
		{
			if(aabb.max.z > _bounds.min.z && aabb.min.z < _centre.z) // falls into back
			{
				_children[TRB]->addToColliders(aabb, colliders);// Top Right Back
			}

			if(aabb.max.z > _centre.z && aabb.min.z < _bounds.max.z) // falls into front
			{
				_children[TRF]->addToColliders(aabb, colliders);// Top Right Front
			}
		}
	}

}



void OctTreeNode::createChildNodes()
{
	for(int i = 0; i < 8; ++i)
	{
		_children[i] = new OctTreeNode(_bounds, _depth + 1, _maxCols, _maxDepth);

		// Set x
		switch(i)
		{
		case TLF:
		case BLF:
		case TLB:
		case BLB:
			// Left = max.x is centre
			_children[i]->_bounds.max.x = _centre.x;
			break;

		default:
			// Right = min.x is centre
			_children[i]->_bounds.min.x = _centre.x;
			break;
		}

		// Set y
		switch(i)
		{
		case BLF:
		case BRF:
		case BLB:
		case BRB:
			// Bottom = max.y is centre
			_children[i]->_bounds.max.y = _centre.y;
			break;

		default:
			// Top = min.y is centre
			_children[i]->_bounds.min.y = _centre.y;
			break;
		}

		// Set z
		switch(i)
		{
		case TLB:
		case TRB:
		case BLB:
		case BRB:
			// Back = max.z is centre
			_children[i]->_bounds.max.z = _centre.z;
			break;

		default:
			// Front = min.z is centre
			_children[i]->_bounds.min.z = _centre.z;
			break;
		}

	}
}

	
void OctTreeNode::destroy()
{
	// If leaf
	if(_children[0] == nullptr) return;

	for(int i = 0; i < 8; ++i)
	{
		_children[i]->destroy();
		delete _children[i];
		_children[i] = nullptr;
	}
}




//#####################
//     OCT TREE
//#####################
OctTree::OctTree()
{
	_root = nullptr;
	_maxCols = 3; // max 3 in one node
	_maxDepth = 5; // max 5 divisions
}

OctTree::~OctTree()
{
	clear();
}



void OctTree::createTree(const std::vector<SPtr_Collider> &colliders)
{
	if(colliders.size() == 0) return;
	clear(); // just in case any old tree left over!

	// Set bounds of root to ensure it exactly encompasses all elements
	AABB rootBounds = colliders[0]->getBounds();
	for(int i = 1; i < colliders.size(); ++i)
	{
		AABB colBounds = colliders[i]->getBounds();
		rootBounds.min.x = std::min(rootBounds.min.x, colBounds.min.x);
		rootBounds.min.y = std::min(rootBounds.min.y, colBounds.min.y);
		rootBounds.min.z = std::min(rootBounds.min.z, colBounds.min.z);
		rootBounds.max.x = std::max(rootBounds.max.x, colBounds.max.x);
		rootBounds.max.y = std::max(rootBounds.max.y, colBounds.max.y);
		rootBounds.max.z = std::max(rootBounds.max.z, colBounds.max.z);
	}

	// Create root node
	_root = new OctTreeNode(rootBounds, 0, _maxCols, _maxDepth);

	// Insert nodes
	for(int i = 0; i < colliders.size(); ++i)
	{
		_root->insert(colliders[i]);
	} 
}

void OctTree::clear()
{
	if(_root != nullptr)
	{
		_root->destroy();
		delete _root;
		_root = nullptr;
	}
}


void OctTree::getCollisionMatrix(std::map<SPtr_Collider, std::set<SPtr_Collider>> &matrix)
{
	if(_root == nullptr) return;
	_root->addToColMatrix(matrix);
}

void OctTree::getCollidersFromAABB(const AABB &aabb, std::set<SPtr_Collider> &colliders)
{
	if(_root == nullptr) return;
	_root->addToColliders(aabb, colliders);
}











// DEBUG
void OctTreeNode::insertDebug(SPtr_Collider collider, SPtr_Collider debugCollider)
{
	bool doMsg = collider == debugCollider;

	// If any child is null then this node is a leaf node
	if(_children[0] == nullptr) // IF LEAF...
	{
		// If not at max depth but putting this collider in will make it greater than max cols
		if(_depth >= _maxDepth)
		{
			if(doMsg) std::cout << "Adding at depth " << _depth << "\n";
			_colliders.push_back(collider);
		}
		else if(_colliders.size() < _maxCols)
		{
			if(doMsg) std::cout << "Adding at depth " << _depth << "\n";
			_colliders.push_back(collider);
		}
		else
		{
			if(doMsg) std::cout << "Splitting at " << _depth << "\n";

			// Create child nodes
			createChildNodes();


			// After division, call insert on all colliders to re-insert them to this node (thus inserting them into correct child node)
			for(int i = 0; i < _colliders.size(); ++i)
			{
				insertDebug(_colliders[i], debugCollider);
			}
			insertDebug(collider, debugCollider); // don't forget this one as well! Obviously not added to _colliders
			_colliders.clear();
		}
	}
	else // IS NOT LEAF ...
	{
		// determine which child node(s) this collider falls into and insert it
		AABB colBounds = collider->getBounds();

		if(doMsg) 
		{
			std::cout << "\n\nColBounds = ";
			colBounds.print();
			std::cout << "\n\n";
		}

		//std::cout << "Col bounds: Min.x " << colBounds.min.x << ", Max.x " << colBounds.max.x;
		//std::cout << "Bounds Min.x " << _bounds.min.x << ", centre.x " <<  _centre.x << "\n";

		if(colBounds.max.x >= _bounds.min.x && colBounds.min.x < _centre.x) // falls into left
		{
			if(colBounds.max.y >= _bounds.min.y && colBounds.min.y < _centre.y) // falls into bottom
			{
				if(colBounds.max.z >= _bounds.min.z && colBounds.min.z < _centre.z) // falls into back
				{
					_children[BLB]->insertDebug(collider, debugCollider);// Bottom Left Back
					if(doMsg) std::cout << "At depth " << _depth << ", recur to Bottom Left Back\n";
				}

				if(colBounds.max.z > _centre.z && colBounds.min.z <= _bounds.max.z) // falls into front
				{
					_children[BLF]->insertDebug(collider, debugCollider);// Bottom Left Front
					if(doMsg) std::cout << "At depth " << _depth << ", recur to Bottom Left Front\n";
				}
			}

			if(colBounds.max.y > _centre.y && colBounds.min.y <= _bounds.max.y) // falls into top
			{
				if(colBounds.max.z >= _bounds.min.z && colBounds.min.z < _centre.z) // falls into back
				{
					_children[TLB]->insertDebug(collider, debugCollider);// Top Left Back
					if(doMsg) std::cout << "At depth " << _depth << ", recur to Top Left Back\n";
				}

				if(colBounds.max.z > _centre.z && colBounds.min.z <= _bounds.max.z) // falls into front
				{
					_children[TLF]->insertDebug(collider, debugCollider);// Top Left Front
					if(doMsg) std::cout << "At depth " << _depth << ", recur to Top Left Front\n";
				}
			}
		}

		if(colBounds.max.x > _centre.x && colBounds.min.x <= _bounds.max.x) // falls into right
		{
			if(colBounds.max.y >= _bounds.min.y && colBounds.min.y < _centre.y) // falls into bottom
			{
				if(colBounds.max.z >= _bounds.min.z && colBounds.min.z < _centre.z) // falls into back
				{
					_children[BRB]->insertDebug(collider, debugCollider);// Bottom Right Back
					if(doMsg) std::cout << "At depth " << _depth << ", recur to Bottom Right Back\n";
				}

				if(colBounds.max.z > _centre.z && colBounds.min.z <= _bounds.max.z) // falls into front
				{
					_children[BRF]->insertDebug(collider, debugCollider);// Bottom Right Front
					if(doMsg) std::cout << "At depth " << _depth << ", recur to Bottom Right Front\n";
				}
			}

			if(colBounds.max.y > _centre.y && colBounds.min.y <= _bounds.max.y) // falls into top
			{
				if(colBounds.max.z >= _bounds.min.z && colBounds.min.z < _centre.z) // falls into back
				{
					_children[TRB]->insertDebug(collider, debugCollider);// Top Right Back
					if(doMsg) std::cout << "At depth " << _depth << ", recur to Top Right Back\n";
				}

				if(colBounds.max.z > _centre.z && colBounds.min.z <= _bounds.max.z) // falls into front
				{
					_children[TRF]->insertDebug(collider, debugCollider);// Top Right Front
					if(doMsg) std::cout << "At depth " << _depth << ", recur to Top Right Front\n";
				}
			}
		}
	}
}