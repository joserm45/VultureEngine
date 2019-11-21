#include "PanelConsole.h"
#include "imgui/imgui.h"
#include "Application.h"

PanelConsole::PanelConsole()
{
	name = "console";
	visibility = true;
	x = 0, y = 780, w = 694, h = 209;
}

PanelConsole::~PanelConsole()
{
	Clear();
}

void PanelConsole::Draw()
{
	/*
	ImGui::SetNextWindowPos({ 0,803 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(694, 209), ImGuiCond_Once);
	*/
	int x, y;
	App->window->GetWinSize(x, y);
	if (resize) {
		ImGui::SetNextWindowPos(ImVec2(this->x, y-h), ImGuiCond_Always);
		w = x - 360;
		ImGui::SetNextWindowSize(ImVec2(w-h, h), ImGuiCond_Always);
		resize = false;
	}
	else
	{
		ImGui::SetNextWindowPos({ 0,803 }, ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(694, 209), ImGuiCond_Once);
	}

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
