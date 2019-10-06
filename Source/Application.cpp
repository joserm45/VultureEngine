#include "Application.h"

#include "parson\parson.h"
#include "PCG\entropy.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	imgui = new ModuleImGui(this);
	load_mesh = new ModuleLoadMesh(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	

	
	// Scenes
	AddModule(scene_intro);
	AddModule(load_mesh);
	AddModule(imgui);
	//AddModule(player);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend())
	{
		delete (*item);
		item++;
	}
}

bool Application::Init()
{
	bool ret = true;

	uint64_t seeds[2];
	entropy_getbytes((void*)seeds, sizeof(seeds));
	pcg32_srandom_r(&random_num, seeds[0], seeds[1]);

	// Load Config.json

	JSON_Value* root_value = json_parse_file("config.json");
	JSON_Object* module_root_object = json_value_get_object(root_value);

	JSON_Object* module_object = json_object_get_object(module_root_object, "Application");
	SetAppName(json_object_get_string(module_object, "Title"));
	SetOrganizationName(json_object_get_string(module_object, "Organization"));
	SetMaxFramerate(json_object_get_number(module_object, "Max FPS"));

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}
	

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}


// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();
	return ret;
}

// ---------------------------------------------

void Application::FinishUpdate()
{
	if (go_to_save == true)
	{
		Save();
		go_to_save = false;
	}
	else if (go_to_load == true)
	{
		Load();
		go_to_load = false;
	}
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();
	
	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}
	return ret;
	
	return true;
}

void Application::Save()
{
	/*JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);*/

	/*if (root_value == NULL)
	{
		//Create Config
		root_value = json_value_init_object();
	}*/

	//root_object = json_value_get_object(root_value);
		// Saving App 

	JSON_Value* root_module_value = json_value_init_object();
	JSON_Object* object_module = json_value_get_object(root_module_value);

	json_object_set_value(object_module, "Application", root_module_value);
	json_object_set_string(object_module, "Title", GetAppName());
	json_object_set_string(object_module, "Organization", GetOrganizationName());
	json_object_set_number(object_module, "Max FPS", GetMaxFramerate());

	for (list<Module*>::const_iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		root_module_value = json_value_init_object();
		object_module = json_value_get_object(root_module_value);

		/*json_object_set_value(root_object, (*item)->GetName(), root_module_value);
		(*item)->SaveStatus(object_module); TODO */
	}

	//json_value_free(root_value);

	App->imgui->AddLogToConsole("Saved with exit!");
}

void Application::Load()
{
	JSON_Value* root_value = json_parse_file("config.json");
	JSON_Object* root_object = nullptr;



	root_object = json_value_get_object(root_value);

	JSON_Value* root_module_value = json_value_init_object();
	JSON_Object* object_module = json_value_get_object(root_module_value);

	for (list<Module*>::const_iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		root_module_value = json_value_init_object();
		object_module = json_value_get_object(root_module_value);

		json_object_set_value(root_object, (*item)->GetName(), root_module_value);
		(*item)->LoadStatus(object_module);
	}

	json_value_free(root_value);
}

uint Application::GenerateRandomNumber() const
{
	return pcg32_random_r(&(App->random_num));
}

void Application::SaveProject()
{
	go_to_save = true;
}

void Application::LoadProject()
{
	go_to_load = true;
}

void Application::SetAppName(const char* name)
{
	appName = new char[CHAR_BUFFER_SIZE];
	strcpy_s((char*)appName, CHAR_BUFFER_SIZE, name);
	
	window->SetTitle(appName);
}

const char* Application::GetAppName() const
{
	return appName;
}

void Application::SetOrganizationName(const char* name)
{
	organizationName = new char[CHAR_BUFFER_SIZE];
	strcpy_s((char*)organizationName, CHAR_BUFFER_SIZE, name);
}

const char* Application::GetOrganizationName() const
{
	return organizationName;
}

void Application::SetMaxFramerate(uint max_framerate)
{
	this->max_framerate = max_framerate;
}

uint Application::GetMaxFramerate() const
{
	return max_framerate;;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}