
#ifndef __GAME_OBJECT_H
#define __GAME_OBJECT_H

#include "Module.h"
#include "Globals.h"
#include "Components.h"
#include <vector>

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