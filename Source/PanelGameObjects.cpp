#include "Application.h"
#include "PanelGameObjects.h"
#include "Application.h"
#include "GameObject.h"
#include "imgui/imgui.h"


PanelGameObjects::PanelGameObjects()
{
	name = "Panel Game Objects";
	active_panel = true;
	x = 0, y = 20, w = 180, h = 800;
}


PanelGameObjects::~PanelGameObjects()
{
}

void PanelGameObjects::Draw()
{
	ImGui::Begin("Scene", &active_panel, ImGuiWindowFlags_None);

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
	else {

		for (uint i = 0; i < game_object->GetNumChilds(); ++i)
		{
			DrawPanelGameObjects(game_object->GetChild(i), node_id);
		}
			
	}
}

