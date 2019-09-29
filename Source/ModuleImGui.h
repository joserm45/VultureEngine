#pragma once
#include "Module.h"
#include <list>

class Panel;
class PanelConfiguration;
class PanelConsole;
class PanelAbout;

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

	update_status HandleMainMenuBar();
	void AddLogToConsole(const char* log);
	void RequestBrowser(const char* url) const;

private:

	//Panels
	list<Panel*> panels;
	PanelConfiguration* config;
	PanelConsole* console;
	PanelAbout* about;

};
