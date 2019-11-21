#include "Application.h"
#include "PanelInspector.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "ModuleScene.h"

PanelInspector::PanelInspector()
{
	name = "inspector";
	visibility = true;
	x = 1016, y = 115, w = 262, h = 557;
}


PanelInspector::~PanelInspector()
{
}

void PanelInspector::Draw()
{
	if (resize) 
	{
		int x, y;
		App->window->GetWinSize(x, y);
		ImGui::SetNextWindowPos(ImVec2(x-w, this->y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
		resize = false;
	}

	ImGui::Begin("Inspector");

	GameObject* focus_game_objetct = GetFocusedGameObject(App->scene_intro->GetRootGameObject());
	if (focus_game_objetct)
		focus_game_objetct->DrawInspector();
	ImGui::End();
}


GameObject* PanelInspector::GetFocusedGameObject(GameObject* root) {

	GameObject* ret = nullptr;

	for (std::vector<GameObject*>::const_iterator it = root->childs.begin(); it < root->childs.end(); it++) {

		if ((*it)->focused)
			return (*it);
		else
			ret = GetFocusedGameObject((*it));

		if (ret) {
			if (ret->focused)
				break;
		}
	}

	return ret;
}