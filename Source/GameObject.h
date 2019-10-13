
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Module.h"
#include "Globals.h"
#include "Components.h"
//#include <vector>

class CompMaterial;
class CompAudio;
class CompMesh;

class GameObject
{
public:

	GameObject(GameObject* parent);
	virtual ~GameObject();

	void Update();
	Components* CreateComponent(TYPECOMP type, int num_mesh , const char* path = "");

	bool IsActive() const;
	void SetActive(bool active);

	GameObject* GetParent();
	GameObject* GetChild(uint i);


	void SetName(const char* new_name);

private:
	const char* name;

	bool active = true;

	GameObject* parent = nullptr;
	//std::vector<GameObject*> childs;

public:

	CompTransform* transform = nullptr;
	CompMaterial* material = nullptr;
	CompAudio* audio = nullptr;
	CompMesh* mesh = nullptr;
};

#endif