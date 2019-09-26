#include "Application.h"
#include "PanelConfiguration.h"

#include "imgui/imgui.h"


PanelConfiguration::PanelConfiguration()
{
	name = "config";
	visibility = true;
}

PanelConfiguration::~PanelConfiguration()
{
}

void PanelConfiguration::Draw()
{
	//Set window position and size
	ImGui::SetNextWindowPos({ 450,100 }, ImGuiWindowFlags_MenuBar);
	ImGui::SetNextWindowSize(ImVec2(320, 700), ImGuiCond_Once);

	//Window
	ImGui::Begin("Configuration", &visibility, ImGuiWindowFlags_None);
	if (ImGui::BeginMenu("Options"))
	{
		ImGui::MenuItem("Set defaults"); 
		ImGui::MenuItem("Save"); 
		ImGui::MenuItem("Load"); 

		ImGui::EndMenu();
	}
	if (ImGui::CollapsingHeader("Application"))
	{ }

	if (ImGui::CollapsingHeader("Window"))
	{ }

	if (ImGui::CollapsingHeader("File System"))
	{ }

	if (ImGui::CollapsingHeader("Input"))
	{ }

	if (ImGui::CollapsingHeader("Hardware"))
	{ }

	ImGui::End();
}


