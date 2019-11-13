#include "Application.h"
#include "Globals.h"
#include "GameObject.h"

#include "Components.h"
#include "CompTransform.h"
#include "CompMaterial.h"
#include "CompMesh.h"

#include "imgui/imgui.h"

#include "MathGeoLib/include/MathGeoLib.h"

GameObject::GameObject()
{
}

GameObject::GameObject(GameObject* root)
{
	this->parent = root;
	name = "Game Object";
	if (root != nullptr)
	{
		root->childs.push_back(this);
	}


	CreateComponent(TRANSFORM,0,"null");
}


GameObject::~GameObject()
{
	for (int i = 0; i < childs.size(); i++)
		RELEASE(childs[i]);
	childs.clear();

	RELEASE(transform);	//CLEAN ALL COMPONENTS
	RELEASE(mesh);
	//RELEASE(material); //crash

	parent = nullptr;
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


const char* GameObject::GetName()
{
	return name;
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
	return childs[number_child];
}

uint GameObject::GetNumChilds() const
{
	return childs.size();
}

void GameObject::PrintPanelGameObject(int& i, bool& clicked) {
	//Pop ID for each tree node
	ImGui::PushID(i);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	
	if (GetNumChilds() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (focused)
		flags |= ImGuiTreeNodeFlags_Selected;

	if (ImGui::TreeNodeEx((void*)(intptr_t)i, flags, name)) 
	{

		if (ImGui::IsItemClicked() && !clicked) {
			App->imgui->AddLogToConsole("Node clicked");
			clicked = true;
			App->scene_intro->GetRootGameObject()->focused = false;
			App->scene_intro->FocusGameObject(this, App->scene_intro->GetRootGameObject()); //Focus
		}

		//Print each child of the gameobject

		for (int j = 0; j < GetNumChilds(); j++) {
			i++;
			GetChild(j)->PrintPanelGameObject(i, clicked);

		}

		ImGui::TreePop();
	}
	//Focus
	if (!clicked && ImGui::IsItemClicked()) {
		LOG("%s node clicked", name);
		clicked = true;
		App->scene_intro->GetRootGameObject()->focused = false;
		App->scene_intro->FocusGameObject(this, App->scene_intro->GetRootGameObject());
	}
	ImGui::PopID();
}

void GameObject::DrawInspector()
{
	if (ImGui::CollapsingHeader("GameObject")) {
		ImGui::PushID(0);
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.7f, 0.8f, 0.0f, 1.0f), name);
		ImGui::PopID();
	}

	if (transform != NULL) {
		ImGui::PushID(1);
		transform->Draw();
		ImGui::PopID();
	}

	if (mesh != NULL) {
		ImGui::PushID(2);
		mesh->Draw();
		ImGui::PopID();
	}

	if (material != NULL) {
		ImGui::PushID(3);
		material->Draw();
		ImGui::PopID();
	}
}

math::float4x4& GameObject::GetLocalMatrix()
{
	return float4x4::FromTRS(transform->position, transform->rotation, transform->scale);
}
math::float4x4 GameObject::GetGlobalMatrix()
{
	float4x4 local_matrix = GetLocalMatrix();
	//transform->GetGlobalMatrix() * local_matrix;
	return local_matrix;
}
void GameObject::SetPosition(float3 position)
{
	transform->position = position;
}
void GameObject::SetRotation(float3 rotation)
{
	transform->rotation = Quat::FromEulerXYZ(rotation.x * DEGTORAD, rotation.y * DEGTORAD, rotation.z *DEGTORAD);
}

