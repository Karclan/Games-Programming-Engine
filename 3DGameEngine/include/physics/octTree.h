#ifndef OCT_TREE_H
#define OCT_TREE_H

#include <vector>
#include <map>
#include <set>

#include "physics\collider.h"
#include "physics\physicsBody.h"



class OctTreeNode
{
	friend class OctTree; // to allow oct tree interface to manage the nodes

public:
	OctTreeNode(AABB bounds, unsigned int depth, unsigned int maxCols, unsigned int maxDepth);

private:
	enum ChildID { TLF, TRF, BLF, BRF, TLB, TRB, BLB, BRB };

	void insert(SPtr_Collider collider); //!< Add to tree, recursively dividing if needed until reached max depth or nodes have fewer than MAX colliders in
	void addToColMatrix(std::map<SPtr_Collider, std::set<SPtr_Collider>> &matrix); //!< Given a map, will poulate with key = collider, value = vector of everything it may collide with (ensuring no duplicates)
	void destroy(); //!< Calls destroy in all children, then deletes them, effectively deleting all nodes except root. Root is then cleared of colliders


	void createChildNodes(); //!< Creates children. Assumes leaf check has already been done so will cause memory leaks if called when not a leaf!
	
	OctTreeNode* _children[8];
	
	unsigned int _depth;
	AABB _bounds;
	unsigned int _maxCols;
	unsigned int _maxDepth;
	glm::vec3 _centre;
	std::vector<SPtr_Collider> _colliders;
};




class OctTree
{
public:
	OctTree();
	~OctTree();

	void createTree(const std::vector<SPtr_Collider> &colliders); //!< Creates the tree based on given vector of colliders. Will auto clear beforehand
	void clear(); //!< Clears oct tree by calling destroy on root, thus recursively calling it on all children
	void getCollisionMatrix(std::map<SPtr_Collider, std::set<SPtr_Collider>> &matrix); //!< Given a map, will poulate with key = collider, value = vector of everything it may collide with (ensuring no duplicates)
	

private:
	OctTreeNode* _root;
	unsigned int _maxCols;
	unsigned int _maxDepth;
	
};




#endif