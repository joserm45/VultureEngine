#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#define QUADTREE_MAX_SIZE 5
#define QUADTREE_MAX_SUBDIVISIONS 5

#include "MathGeoLib/include/Geometry/AABB.h"
#include <list>
#include <vector>

#include "Globals.h"

class GameObject;

enum QuadSubdivide
{
	TOP_LEFT,
	TOP_RIGHT,
	BOT_LEFT,
	BOT_RIGHT
};

class QuadtreeNode
{
public:

	QuadtreeNode(const math::AABB bbox, QuadtreeNode* parent = nullptr);
	virtual ~QuadtreeNode();

	bool IsLeaf();

	void Insert(GameObject* to_insert);
	void Remove(GameObject* to_remove); 

	void Subdivide();
	void DistributeObjects();
	void DebugDraw();

	void DrawQuadtree();

	template<typename TYPE>
	inline void Intersect(std::vector<GameObject*>& objects, const TYPE& primitive);

	template<typename Type>
	inline void CollectIntersections(std::vector<GameObject*>& objects, Type & primitive);

public:

	math::AABB bbox;
	QuadtreeNode* parent = nullptr;
	QuadtreeNode* childs[4]; 
	std::vector<GameObject*> objects;

	uint subdivisions = 0;
};

class Quadtree
{
public:

	Quadtree();
	~Quadtree();

	void Boundaries(math::AABB limits);
	void Clear();

	void Remove(GameObject* to_remove);
	void Insert(GameObject* gameObject);
	void DebugDraw();

	template<typename TYPE>
	inline void Intersect(std::vector<GameObject*>& objects, const TYPE& primitive);
	template<typename Type>
	inline void CollectIntersections(std::vector<GameObject*>& objects, Type & primitive);

public:
	float3 min_point;
	float3 max_point;
	QuadtreeNode* root = nullptr;
};

template<typename TYPE>
inline void Quadtree::Intersect(std::vector<GameObject*>& objects, const TYPE& primitive)
{
	if (root)
		root->Intersect(objects, primitive);
}

template<typename TYPE>
inline void QuadtreeNode::Intersect(std::vector<GameObject*>& objects, const TYPE& primitive)
{
	if (primitive.Intersects(bbox))
	{
		for (std::vector<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			if (std::find(objects.begin(), objects.end(), (*it)) == objects.end()) //Check that the element is not in the list
				if (primitive.Intersects((*it)->BBox))
					objects.push_back(*it);

		}
		for (int i = 0; i < 4; ++i)
			if (childs[i] != nullptr) childs[i]->Intersect(objects, primitive);

	}
}
template<typename Type>
inline void QuadtreeNode::CollectIntersections(std::vector<GameObject*>& gameObjects, Type& primitive)
{
	if (primitive.Intersects(bbox))
	{
		for (std::vector<GameObject*>::const_iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if (primitive.Intersects(*(*it)->bbox))
				gameObjects.push_back(*it);
		}

		if (!IsLeaf())
		{
			for (uint i = 0; i < 4; ++i)
				childs[i]->CollectIntersections(gameObjects, primitive);
		}
	}
}

template<typename Type>
inline void Quadtree::CollectIntersections(std::vector<GameObject*>& gameObjects, Type& primitive)
{
	if (root != nullptr)
		root->CollectIntersections(gameObjects, primitive);
}

#endif // !__QUADTREE__
