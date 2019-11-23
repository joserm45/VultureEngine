#include "Application.h"
#include "ModuleImGui.h"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "ImGuizmo\ImGuizmo.h"

#include "Panel.h"
#include "PanelConfiguration.h"
#include "PanelConsole.h"
#include "PanelAbout.h"
#include "PanelInspector.h"
#include "PanelGameObjects.h"
#include "PanelState.h"

ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ImGui";
}

ModuleImGui::~ModuleImGui()
{
}

bool ModuleImGui::Init()
{

	IMGUI_CHECKVERSION();
	//imGui init
	
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	//Window Style
	ImGui::StyleColorsLight();

	config = new PanelConfiguration();
	panels.push_back(config);
	console = new PanelConsole();
	panels.push_back(console);
	about = new PanelAbout();
	panels.push_back(about);
	game_objects = new PanelGameObjects();
	panels.push_back(game_objects);
	inspector = new PanelInspector();
	panels.push_back(inspector);
	state = new PanelState();
	panels.push_back(state);

	AddLogToConsole("Initialized ImGui Correctly");

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	//Stablish new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	ImGuizmo::BeginFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{

	//Draw panel 
	list<Panel*>::const_iterator panel = panels.begin();
	while (panel != panels.end())
	{
		if ((*panel)->IsEnabled())
			(*panel)->Draw();
		panel++;
	}

	update_status status = HandleMainMenuBar();

	if ((App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT) && console->visibility != false)
		AddLogToConsole("TESTING");
	return status;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	//Render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	//Clearing up imGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	//if(App->imgui->q)
	ImGui::DestroyContext();

	//CleanUp panels
	list<Panel*>::const_iterator panel = panels.begin();

	while (panel != panels.end())
	{
		if (&panel != nullptr) {
			delete *panel;
			panels.remove(*panel++);
			continue;
		}
		panel++;
	}

	config = nullptr;
	console = nullptr;


	return true;
}

void ModuleImGui::Draw()
{
	list<Panel*>::const_iterator panel = App->imgui->panels.begin();
	while (panel != App->imgui->panels.end())
	{
		if ((*panel)->IsActive())
			(*panel)->Draw();
		panel++;
	}
}

bool ModuleImGui::IsMouseHoveringWindow()
{
	return ImGui::GetIO().WantCaptureMouse;
}

update_status ModuleImGui::HandleMainMenuBar()
{
	static bool quit = false;
	static bool demo = false;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Quit", "ESC", &quit);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console","0", console->IsEnabled()))
				console->ToggleVisibility();

			if (ImGui::MenuItem("Configuration","1", config->IsEnabled()))
				config->ToggleVisibility();

			if (ImGui::MenuItem("Scene GO", "2", game_objects->IsEnabled()))
				game_objects->ToggleVisibility();

			if (ImGui::MenuItem("Play/Pause", "3", state->IsEnabled()))
				state->ToggleVisibility();

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Gui Demo"))
			{
				demo = !demo;
			}

			if (ImGui::MenuItem("Documentation"))
				RequestBrowser("https://github.com/joserm45/VultureEngine");

			if (ImGui::MenuItem("About", "", about->IsEnabled()))
			{
				about->ToggleVisibility();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Shapes"))
		{
			if (ImGui::MenuItem("Cube"))
			{
				App->importer->LoadMesh(nullptr, true, 1);
			}
			if (ImGui::MenuItem("Sphere"))
			{
				App->importer->LoadMesh(nullptr, true, 2);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (quit == true) 
		//return UPDATE_STOP;
	if (demo == true)
	{
		ImGui::ShowDemoWindow(&demo);
	}
	return UPDATE_CONTINUE;
}

void ModuleImGui::ResizePanels()
{
	//Resize panels
	list<Panel*>::const_iterator panel = panels.begin();
	while (panel != panels.end())
	{
		if ((*panel)->IsEnabled())
			(*panel)->IsResizePanel();
		panel++;
	}
}

void ModuleImGui::AddLogToConsole(const char* log)
{
	console->ShowLog(log);
}

void ModuleImGui::AddLogToConsole(const char* log, const char* dlog)
{
	console->ShowLog(log, dlog);
}

void ModuleImGui::RequestBrowser(const char* url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}