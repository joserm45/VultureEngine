#include "Application.h"
#include "PanelInspector.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "ModuleScene.h"

PanelInspector::PanelInspector()
{
	name = "inspector";
	active_panel = true;
	x = 1261, y = 20, w = 303, h = 794;
}


PanelInspector::~PanelInspector()
{
}

void PanelInspector::Draw()
{
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