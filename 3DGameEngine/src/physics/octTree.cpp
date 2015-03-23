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
		// If not at max depth but putting this collider in will make it greater than max cols
		if(_depth < _maxDepth && _colliders.size()+1 > _maxCols)
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
		else
		{
			_colliders.push_back(collider);
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
		for(int i = 0; i < _colliders.size(); ++i)
		{
			for(int j = i+1; j < _colliders.size(); ++j)
			{
				matrix[_colliders[i]].insert(_colliders[j]);
			}
		}

	}
	else // not leaf, call on children
	{
		for(int i = 0; i < 8; ++i)
		{
			_children[i]->addToColMatrix(matrix);
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
		_root = nullptr;
	}
}


void OctTree::getCollisionMatrix(std::map<SPtr_Collider, std::set<SPtr_Collider>> &matrix)
{
	if(_root == nullptr) return;
	_root->addToColMatrix(matrix);
}