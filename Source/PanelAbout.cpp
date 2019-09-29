#include "PanelAbout.h"

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
	ImGui::SetNextWindowSize(ImVec2(450, 650), ImGuiCond_Once);

	//Window
	ImGui::Begin("About", &visibility, ImGuiWindowFlags_None);

	ImGui::Text("Vulture Engine v0.1");
	ImGui::Text("The next generation 3D engine.");
	ImGui::Text("Made by Jose Rodriguez & Manav Lakhwani.");
	ImGui::Text("\n");

	ImGui::Text("3rd party libraries used:");
	ImGui::Text(" * SDL 2.0.6"); 
	ImGui::Text(" * Glew 2.0.0"); 
	ImGui::Text(" * ImGui 1.51"); 
	ImGui::Text(" * MathGeoLib 1.5"); 
	ImGui::Text(" * OpenGL 3.1");
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