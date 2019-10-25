#include "Application.h"
#include "Globals.h"
#include "GameObject.h"

#include "Components.h"
#include "CompTransform.h"
#include "CompMaterial.h"
#include "CompMesh.h"

#include "imgui/imgui.h"



GameObject::GameObject(GameObject* root)
{
	this->parent = root;
	if (root != nullptr)
		//root->childs.push_back(this);

	CreateComponent(TRANSFORM,5,"d");
}


GameObject::~GameObject()
{
	for (int i = 0; i < childs.size(); i++)
		RELEASE(childs[i]);
	childs.clear();

	RELEASE(transform, 0,"");	//CLEAN ALL COMPONENTS
	
}

void GameObject::Update()
{
}

Components* GameObject::CreateComponent(TYPECOMP type, int num_mesh, const char* path )
{
	Components* new_component = nullptr;

	switch (type) 
	{
		case TRANSFORM:
			if (transform == nullptr) {
				transform = new CompTransform(this);
				new_component = transform; 
			}
			break;
		
		case MATERIAL:
			if (material == nullptr) {
				material = new CompMaterial(this, path);
				new_component = material;
			}
			break;

		case MESH:
			if (mesh == nullptr) {
				mesh = new CompMesh(this, path, num_mesh);
				new_component = mesh;
			}
			break;

		case DEFAULT:
			return nullptr;
			break;
		}
	return new_component;
}

bool GameObject::IsActive() const
{
	return active;
}

void GameObject::SetActive(bool active)
{
	this->active = active;
}


void GameObject::SetName(const char* new_name) 
{
	name = new_name;
}

GameObject* GameObject::GetParent()
{
	return parent;
}

GameObject* GameObject::GetChild(uint number_child)
{
	//return childs[number_child];
	return 0;
}



