
#ifndef __GAME_OBJECT_H
#define __GAME_OBJECT_H

#include "Module.h"
#include "Globals.h"
#include "Components.h"
#include <vector>
#include "MathGeoLib/include/MathGeoLib.h"

class CompTransform;
class CompMaterial;
class CompMesh;

class GameObject
{
public:

	GameObject();
	GameObject(GameObject* parent);
	virtual ~GameObject();

	void Update();
	Components* CreateComponent(TYPECOMP type, int num_mesh , const char* path = "");

	bool IsActive() const;
	void SetActive(bool active);

	GameObject* GetParent();
	GameObject* GetChild(uint i);
	uint GetNumChilds() const;

	const char* GetName();
	void SetName(const char* new_name);
	void GameObject::PrintPanelGameObject(int& i, bool& clicked);
	void GameObject::DrawInspector();

	//transform
	math::float4x4& GetLocalMatrix();
	math::float4x4 GetGlobalMatrix();
	void SetPosition(float3 position);
	void SetRotation(float3 rotation);

private:

	const char* name;

	bool active = true;


	GameObject* parent = nullptr;


public:
	bool focused = false;
	std::vector<GameObject*> childs;

	CompTransform* transform = nullptr;
	CompMaterial* material = nullptr;
	CompMesh* mesh = nullptr;
};

#endif