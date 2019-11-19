#include "Quadtree.h"
#include "glew\include\GL\glew.h"
#include "Globals.h"
#include "GameObject.h"

#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"

QuadtreeNode::QuadtreeNode(const math::AABB bbox, QuadtreeNode* parent) : bbox(bbox), parent(parent)
{
	for (uint i = 0; i < 4; ++i)
	{
		childs[i] = nullptr;
	}

}

QuadtreeNode::~QuadtreeNode()
{
}

bool QuadtreeNode::IsLeaf() //only if you are a leaf you subdivide by 4 once you get out of space
{
	if (childs[0] == nullptr)
	{
		return true;
	}
	return false;
}

void QuadtreeNode::Insert(GameObject * gameObject)
{
	//if node has space -> pushback in the list
	if (IsLeaf() && (objects.size() < QUADTREE_MAX_SIZE ||
		(bbox.HalfSize().LengthSq() <= QUADTREE_MAX_SUBDIVISIONS * QUADTREE_MAX_SUBDIVISIONS)))
	{
		objects.push_back(gameObject);
	}

	else
	{
		if (IsLeaf())
		{
			Subdivide();

			objects.push_back(gameObject);
			// All gameobjects of the father node have to be inside at least one of the childs
			DistributeObjects();
		}
		else
		{
			for (uint i = 0; i < 4; i++)
			{
				if (childs[i]->bbox.Intersects(gameObject->BBox))
				{
					childs[i]->Insert(gameObject);
				}
			}
		}
	}
}

void QuadtreeNode::Remove(GameObject * to_remove)
{
	std::list<GameObject*>::iterator it = std::find(objects.begin(), objects.end(), to_remove);

	if (it != objects.end())
	{
		objects.erase(it);
	}
	if (!IsLeaf())
	{
		for (uint i = 0; i < 4; i++)
		{
			childs[i]->Remove(to_remove);
		}
	}
}

void QuadtreeNode::Subdivide()
{
	//dividing by 4 the box
	math::float3 size = bbox.Size();
	math::float3 new_size(size.x*0.5f, size.y, size.z*0.5f);
	math::float3 center = bbox.CenterPoint();
	math::float3 new_center;

	math::AABB new_bbox;

	//up left quad
	new_center.Set(center.x - new_size.x * 0.5f, center.y, center.z - new_size.z * 0.5f);
	new_bbox.SetFromCenterAndSize(new_center, new_size);
	childs[TOP_LEFT] = new QuadtreeNode(new_bbox, this);

	// up right quad
	new_center.Set(center.x + new_size.x * 0.5f, center.y, center.z - new_size.z * 0.5f);
	new_bbox.SetFromCenterAndSize(new_center, new_size);
	childs[TOP_RIGHT] = new QuadtreeNode(new_bbox, this);

	//bot left quad
	new_center.Set(center.x + new_size.x * 0.5f, center.y, center.z + new_size.z* 0.5f);
	new_bbox.SetFromCenterAndSize(new_center, new_size);
	childs[BOT_LEFT] = new QuadtreeNode(new_bbox, this);

	// bot right quad
	new_center.Set(center.x - new_size.x * 0.5f, center.y, center.z + new_size.z* 0.5f);
	new_bbox.SetFromCenterAndSize(new_center, new_size);
	childs[BOT_RIGHT] = new QuadtreeNode(new_bbox, this);


	childs[TOP_LEFT]->subdivisions = childs[TOP_RIGHT]->subdivisions = childs[BOT_LEFT]->subdivisions = childs[BOT_RIGHT]->subdivisions = subdivisions + 1;
}

void QuadtreeNode::DistributeObjects()
{
	GameObject* object = nullptr;

	std::list<GameObject*>::iterator it;
	for (it = objects.begin(); it != objects.end();)
	{
		object = *it;
		bool intersecting[4];
		uint num_intersections = 0;

		for (uint i = 0; i < 4; i++)
		{
			if (intersecting[i] = childs[i]->bbox.Intersects(object->BBox))
			{
				num_intersections++;
			}
		}

		if (num_intersections == 4)
		{
			it++;
		}
		else
		{
			// Erase game object from father list and add it to childs nodes
			it = objects.erase(it);
			for (uint i = 0; i < 4; i++)
			{
				if (intersecting[i])
				{
					// Insert the Game Object into correct child
					childs[i]->Insert(object);
				}
			}
		}
	}
}

void QuadtreeNode::DrawQuadtree()
{
	glBegin(GL_LINES);
	glLineWidth(1.0f);
	glColor4f(0.7f, 1.0f, 1.0f, 0.6f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(bbox.Edge(i).a.x, bbox.Edge(i).a.y, bbox.Edge(i).a.z);
		glVertex3f(bbox.Edge(i).b.x, bbox.Edge(i).b.y, bbox.Edge(i).b.z);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnd();

	if (childs[0] != nullptr)
	{
		for (uint i = 0; i < 4; i++)
		{
			childs[i]->DrawQuadtree();
		}
	}
}


Quadtree::Quadtree()
{
}

Quadtree::~Quadtree()
{
	Clear();
}

void Quadtree::Boundaries(math::AABB limits)
{
	Clear();
	root = new QuadtreeNode(limits);
}

void Quadtree::Clear()
{
	delete root;
	root = nullptr;
}

void Quadtree::Remove(GameObject * to_remove)
{
	if (root != nullptr)
	{
		root->Remove(to_remove);
	}
}

void Quadtree::Insert(GameObject * gameObject)
{
	if (root != nullptr && &gameObject->BBox != nullptr)
	{
		if (gameObject->BBox.IsFinite())
		{
			if (gameObject->BBox.Intersects(root->bbox))
			{
				root->Insert(gameObject);
			}
		}
	}

}