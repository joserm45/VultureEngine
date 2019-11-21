#include "PanelState.h"
#include "Application.h"
#include "imgui/imgui.h"


PanelState::PanelState()
{
	name = "Play/Pause";
	visibility = true;
	x = 0, y = 18, w = 783, h = 95;

}


PanelState::~PanelState()
{
}

void PanelState::Draw()
{
	if (resize) 
	{
		int x, y;
		App->window->GetWinSize(x, y);
		this->w = x;

		ImGui::SetNextWindowPos(ImVec2(this->x, this->y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(this->w, this->h), ImGuiCond_Always);
		resize = false;
	}
	static const char* play = "Play";
	static const char* pause = "Pause";

	//static uint time_scale_width = 100;

	EngineState engine_state = App->GetState();
	ImGui::Begin("Play/Pause", &visibility, ImGuiWindowFlags_None);

	// Play/Pause
	ImGui::SetCursorPos({ (float)(w / 2 - 35), (float)(h / 3) });

	//Play/Stop
	if (ImGui::Button(play, { 65, 40 })) {
		if (engine_state != ENGINE_STATE_EDITOR) {
			App->Stop();
			play = "Play";
			pause = "Pause";
		}
		else {
			if (App->Play())
				play = "Stop";
		}
	}

	//Pause
	ImGui::SetCursorPos({ (float)(w / 2 + 35), (float)(h / 3) });
	if (ImGui::Button(pause, { 65, 40 })) {
		if (engine_state == ENGINE_STATE_PLAY)
			pause = "Unpaused";
		else if (engine_state == ENGINE_STATE_PAUSE)
			pause = "Pause";

		App->Pause();
	}


	//Time scale slider
	/*ImGui::PushItemWidth(time_scale_width);
	ImGui::SetCursorPos({ (float)(w / 2 + time_scale_width), (float)(h / 2.5f) });
	ImGui::SliderFloat("Time scale", &App->time->time_scale, 0.0f, 2.0f, "%.1f");*/

	ImGui::End();

}