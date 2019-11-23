
#ifndef __GAME_OBJECT_H
#define __GAME_OBJECT_H

#include "Module.h"
#include "Globals.h"
#include "Components.h"
#include <vector>
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/Geometry/OBB.h"

class CompTransform;
class CompMaterial;
class CompMesh;
class CompCamera;

class GameObject
{
public:

	GameObject();
	GameObject(GameObject* parent);
	virtual ~GameObject();

	void Update();
	Components* CreateComponent(TYPECOMP type, int num_mesh , const char* path = "");

	bool IsVisible() const;
	void SetVisibility(bool active);

	GameObject* GetParent();
	GameObject* GetChild(uint i);
	uint GetNumChilds() const;
	bool IsStatic() const;
	const char* GetName();
	void SetName(const char* new_name);
	void PrintPanelGameObject(int& i, bool& clicked);
	void DrawInspector();

	void Draw();

	//transform
	math::float4x4& GetLocalMatrix();
	math::float4x4 GetGlobalMatrix();
	void SetPosition(float3 position);
	void SetRotation(float3 rotation);
	void SetScale(float3 scale);

	void CreateBBox();
	void DrawBBox();
	void UpdateBoundingBox();
	void ShowGizmos();

	math::AABB BBox;
	math::OBB obb;
private:

	const char* name;

	bool active = true;

	GameObject* parent = nullptr;


public:
	bool game_object_static = false;
	bool focused = false;
	std::vector<GameObject*> childs;

	CompTransform* transform = nullptr;
	CompMaterial* material = nullptr;
	CompMesh* mesh = nullptr;
	CompCamera* camera = nullptr;
};

#endif