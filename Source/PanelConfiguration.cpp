#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "PanelConfiguration.h"

#include "imgui/imgui.h"


PanelConfiguration::PanelConfiguration()
{
	name = "Panel config";
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
	{ 
		static char AppName[CHAR_BUFFER_SIZE];
		static char OrgName[CHAR_BUFFER_SIZE];

		// GET Name Engine
		if (App->GetAppName() != NULL)
			strcpy_s(AppName, CHAR_BUFFER_SIZE, App->GetAppName());

		if(ImGui::InputText("App Name", AppName, IM_ARRAYSIZE(AppName)) != NULL);
			App->SetAppName(AppName);

		if (App->GetOrganizationName() != NULL)
			strcpy_s(OrgName, CHAR_BUFFER_SIZE, App->GetOrganizationName());

		if(ImGui::InputText("Organization", OrgName, IM_ARRAYSIZE(OrgName)));
			App->SetOrganizationName(OrgName);

	
		static int maxFPS = App->GetMaxFramerate();
		if (ImGui::SliderInt("Max FPS", &maxFPS, 0, 125))
			App->SetMaxFramerate(maxFPS);
			
		ImGui::Text("Limit Framerate:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 0, 1, 1),"%d", maxFPS);

		//fps and ms graphs
		if (fps_log.size() != 100)
		{
			fps_log.push_back(App->GetFPS());
			ms_log.push_back(App->GetMS());
		}

		else
		{
			fps_log.erase(fps_log.begin());
			fps_log.push_back(App->GetFPS());

			ms_log.erase(ms_log.begin());
			ms_log.push_back(App->GetMS());
		}

		
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

		
		sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
		
	}

	if (ImGui::CollapsingHeader("Window"))
	{ 
		static bool WindowActive = true;
		if (ImGui::Checkbox("Active", &WindowActive))
		{
		}

		char icon[15] = "*default*";
		ImGui::Text("Icon:");
		ImGui::SameLine();
		ImGui::Text("%s", icon);

		if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0.0f, 1.0f))
		{
			App->window->ChangeBrightness(App->window->brightness);
		}
			
		if (ImGui::SliderInt("Width", &App->window->width_win, 640, 1600))
		{
			App->window->ChangeWidth(App->window->width_win);
		}

		if (ImGui::SliderInt("Height", &App->window->height_win, 480, 1400))
		{
			App->window->ChangeHeight(App->window->height_win);
		}

		//TO DO ??
		static int RefRate = 60;
		ImGui::Text("Refresh rate:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 0, 1, 1), "%d", RefRate);


		if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
		{
			App->window->SetFullscreen(App->window->fullscreen);
		}
		ImGui::SameLine();

		//static bool resizable = WIN_RESIZABLE;
		if (ImGui::Checkbox("Resizable", &App->window->resizable))
		{
			//TO DO
			App->window->SetResizable(App->window->resizable);
			
		}

		if (ImGui::Checkbox("Borderless", &App->window->borderless))
		{
			App->window->SetBorderless(App->window->borderless);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &App->window->FullDesktop))
		{
			App->window->SetFullDesktop(App->window->FullDesktop);
		}
	}

	if (ImGui::CollapsingHeader("File System"))
	{ 
	
	}

	if (ImGui::CollapsingHeader("Input"))
	{ 
	
	}

	if (ImGui::CollapsingHeader("Hardware"))
	{ 
		static bool HardwareActive = true;
		if (ImGui::Checkbox("Active", &HardwareActive))
		{
		}

		SDL_version linked;
		SDL_GetVersion(&linked);
		ImGui::Text("SDL Version:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 0, 1, 1), "%d.%d.%d",linked.major, linked.minor, linked.patch);

		ImGui::Separator();

		ImGui::Text("CPUs:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 0, 1, 1), "(Cache: %dkb)", SDL_GetCPUCacheLineSize());

		static float ram = SDL_GetSystemRAM() / 1000;
		ImGui::Text("System RAM:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 0, 1, 1), "%.2fGb", ram);

		ImGui::Text("Caps:");
		if (SDL_Has3DNow() == true)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 0, 1, 1), "3DNow,");
		}
		if (SDL_HasAltiVec() == true)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 0, 1, 1), "AltiVec,");
		}
		if (SDL_HasAVX() == true)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 0, 1, 1), "AVX,");
		}
		if (SDL_HasAVX2() == true)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 0, 1, 1), "AVX2,");
		}
		if (SDL_HasMMX() == true)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 0, 1, 1), "MMX,");
		}
		if (SDL_HasRDTSC() == true)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 0, 1, 1), "RDTSC,");
		}
		ImGui::Text("\n");
		if (SDL_HasSSE() == true)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 0, 1, 1), "SSE,");
		}
		if (SDL_HasSSE2() == true)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 0, 1, 1), "SSE2,");
		}
		if (SDL_HasSSE3() == true)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 0, 1, 1), "SSE3,");
		}
		if (SDL_HasSSE41() == true)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 0, 1, 1), "SSE41,");
		}
		if (SDL_HasSSE42() == true)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 0, 1, 1), "SSE42,");
		}

		ImGui::Separator();
	}

	ImGui::End();
}


