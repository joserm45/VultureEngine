#include "PanelAbout.h"

#include <windows.h>
#include <shellapi.h>


PanelAbout::PanelAbout()
{
	name = "about";
	visibility = false;
}

PanelAbout::~PanelAbout()
{
}

void PanelAbout::Draw()
{
	//Set window position and size
	ImGui::SetNextWindowPos(ImVec2(400,75), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(460, 660), ImGuiCond_Once);

	//Window
	ImGui::Begin("About", &visibility, ImGuiWindowFlags_None);

	ImGui::Text("Vulture Engine v0.1");
	ImGui::Text("The next generation 3D engine.");
	ImGui::Text("Made by");
	ImGui::SameLine();
	if (ImGui::Button("Jose Rodriguez"))
		ShellExecute(NULL, "open", "https://github.com/joserm45", NULL, NULL, SW_SHOWNORMAL);
	ImGui::SameLine();
	ImGui::Text("&");
	ImGui::SameLine();
	if (ImGui::Button("Manav Lakhwani."))
		ShellExecute(NULL, "open", "https://github.com/manavld", NULL, NULL, SW_SHOWNORMAL);

	ImGui::Text("\n");

	ImGui::Text("3rd party libraries used:");
	if (ImGui::Button(" * SDL 2.0.6"))
		ShellExecute(NULL, "open", "https://www.libsdl.org/", NULL, NULL, SW_SHOWNORMAL);
	if (ImGui::Button(" * Glew 2.0.0"))
		ShellExecute(NULL, "open", "http://glew.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);
	if (ImGui::Button(" * ImGui 1.51"))
		ShellExecute(NULL, "open", "https://github.com/ocornut/imgui", NULL, NULL, SW_SHOWNORMAL);
	if (ImGui::Button(" * MathGeoLib 1.5"))
		ShellExecute(NULL, "open", "https://github.com/juj/MathGeoLib", NULL, NULL, SW_SHOWNORMAL);
	if (ImGui::Button(" * OpenGL 3.1"))
		ShellExecute(NULL, "open", "https://www.opengl.org/?", NULL, NULL, SW_SHOWNORMAL);
	ImGui::Text("\n");

	ImGui::Text("License");
	ImGui::Text("\n");

	ImGui::Text("MIT License");
	ImGui::Text("\n");

	ImGui::Text("Copyright (c) 2019 Jose Rodriguez & Manav Lakhwani");
	ImGui::Text("\n");

	ImGui::Text("Permission is hereby granted, free of charge, to any person\nobtaining a copy of this software and associated\ndocumentation files(the 'Software'), to deal in the Software\nwithout restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense,\nand/or sell copies of the Software, and to permit persons to\nwhom the Software is furnished to do so, subject to the\nfollowing conditions: ");
	ImGui::Text("\n");

	ImGui::Text("The above copyright notice and this permission notice shall\nbe included in all copies or substantial portions of the\nSoftware.");
	ImGui::Text("\n");

	ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY\nKIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE\nWARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR\nPURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR\nCOPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,\nARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE\nOR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");

	ImGui::End();
}