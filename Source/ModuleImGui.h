#pragma once
#include "Module.h"
#include <list>

class Panel;
class PanelConfiguration;
class PanelConsole;
class PanelAbout;
class PanelGameObjects;
class PanelInspector;
class PanelState;

using namespace std;

class ModuleImGui : public Module
{
public:
	
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Draw();
	update_status HandleMainMenuBar();
	void ResizePanels();
	void AddLogToConsole(const char* log);
	void AddLogToConsole(const char* log, const char* dlog);
	void RequestBrowser(const char* url) const;

	list<Panel*> panels;
private:

	//Panels

	PanelConfiguration* config = nullptr;
	PanelConsole* console = nullptr;
	PanelAbout* about = nullptr;
	PanelGameObjects* game_objects = nullptr;
	PanelInspector* inspector = nullptr;
	PanelState* state = nullptr;
};
