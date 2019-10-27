#include "PanelConsole.h"
#include "imgui/imgui.h"


PanelConsole::PanelConsole()
{
	name = "console";
	visibility = true;
}

PanelConsole::~PanelConsole()
{
	Clear();
}

void PanelConsole::Draw()
{
	//Set window position and size
	ImGui::SetNextWindowPos({ 0,803 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(694, 209), ImGuiCond_Once);

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

void PanelConsole::ShowLog(const char* log, const char* dlog)
{
	buff.appendf(log);
	buff.appendf(dlog);
	buff.appendf("\n");
	scroll = true;
}

void PanelConsole::Clear()
{
	buff.clear();
}
