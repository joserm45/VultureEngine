#include "Application.h"
#include "PanelGameObjects.h"
#include "Application.h"
#include "GameObject.h"
#include "imgui/imgui.h"


PanelGameObjects::PanelGameObjects()
{
	name = "Panel Game Objects";
	visibility = true;
	x = 1, y = 115, w = 236, h = 653;
}


PanelGameObjects::~PanelGameObjects()
{
}

void PanelGameObjects::Draw()
{
	if (resize)
	{
		int x, y;
		App->window->GetWinSize(x, y);
		ImGui::SetNextWindowPos(ImVec2(this->x , this->y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
		resize = false;
	}
	ImGui::Begin("Scene", &visibility, ImGuiWindowFlags_None);

	int node_index = 0;

	DrawPanelGameObjects(App->scene_intro->GetRootGameObject(), node_index);

	ImGui::End();
}

void PanelGameObjects::DrawPanelGameObjects(GameObject* game_object, int& node_id)
{
	if (game_object == App->scene_intro->GetRootGameObject()) 
	{
		bool click = false;
		
		game_object->PrintPanelGameObject(node_id, click);
		node_id++;
	}
	else 
	{
		for (uint i = 0; i < game_object->GetNumChilds(); ++i)
		{
			DrawPanelGameObjects(game_object->GetChild(i), node_id);			
		}
			
	}
}

