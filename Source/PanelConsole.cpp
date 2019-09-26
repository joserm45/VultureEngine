#include "PanelConsole.h"
#include "imgui/imgui.h"


PanelConsole::PanelConsole()
{
	name = "console";
	visibility = true;
}

PanelConsole::~PanelConsole()
{
}

void PanelConsole::Draw()
{
	//Set window position and size
	ImGui::SetNextWindowPos({ 100,50 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500, 250), ImGuiCond_Once);

	static bool autom_scroll = true;

	//Window
	ImGui::Begin("Console", &visibility, ImGuiWindowFlags_None);

	ImGui::TextUnformatted(buff.begin());

	//Scrolling Logs

	if (scroll && autom_scroll == true) {
		ImGui::SetScrollHere(1.0f);
		scroll = false;
	}

	ImGui::End();
}

void PanelConsole::ShowLog(const char* log)
{
	buff.appendf(log);
	buff.appendf("\n");
	scroll = true;
}

void PanelConsole::Clear()
{
	buff.clear();
}
