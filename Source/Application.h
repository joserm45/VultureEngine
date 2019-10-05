#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ModuleLoadMesh.h"

#include "PCG\pcg_variants.h"

class Application
{
public:

	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleImGui* imgui = nullptr;
	ModuleLoadMesh* load_mesh = nullptr;

	pcg32_random_t random_num;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;
	
	bool go_to_save = false;
	bool go_to_load = false;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	uint GenerateRandomNumber() const;
	void SaveProject();
	void LoadProject();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void Save();
	void Load();
};

extern Application *App;