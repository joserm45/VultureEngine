#include "Application.h"
#include "ModuleImGui.h"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Panel.h"
#include "PanelConfiguration.h"
#include "PanelConsole.h"


ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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

	AddLogToConsole("Initializing ImGui");

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	//Stablish new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{

	//Draw panel or not
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
	ImGui::DestroyContext();

	//CleanUp panels
	list<Panel*>::const_iterator panel = panels.begin();

	while (panel != panels.end())
	{
		if (&panel != nullptr) {
			delete &panel;
			panels.remove(*panel++);
			continue;
		}
		panel++;
	}

	config = nullptr;
	console = nullptr;


	return true;
}

update_status ModuleImGui::HandleMainMenuBar()
{
	static bool quit = false;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Quit", "ESC", &quit);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console","", console->IsEnabled()))
				console->ToggleVisibility();

			if (ImGui::MenuItem("Configuration","", config->IsEnabled()))
				config->ToggleVisibility();

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Gui Demo"))
			{}

			if (ImGui::MenuItem("Documentation"))
				RequestBrowser("https://github.com/joserm45/VultureEngine");

			if (ImGui::MenuItem("About"))
			{}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (quit == true) 
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

void ModuleImGui::AddLogToConsole(const char * log)
{
	console->ShowLog(log);
}

void ModuleImGui::RequestBrowser(const char* url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}