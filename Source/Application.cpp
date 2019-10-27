#include "Application.h"

#include "parson\parson.h"
#include "PCG\entropy.h"

Application::Application()
{
	frames = 0;
	PERF_START(ptimer);

	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleScene(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	imgui = new ModuleImGui(this);
	importer = new ModuleImport(this);


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	

	
	// Scenes
	AddModule(scene_intro);
	AddModule(importer);
	AddModule(imgui);
	//AddModule(panel);

	// Renderer last!
	AddModule(renderer3D);

	cap = "ON";

	PERF_PEEK(ptimer);
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
	PERF_START(ptimer);

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

	//framerete_cap = ; 
	framerete_ms = 1000 / framerete_cap;

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
	

	PERF_PEEK(ptimer);
	//ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	//dt = (float)ms_timer.Read() / 1000.0f;
	//ms_timer.Start();
	frame_count++;
	last_sec_frame_count++;
	dt = frame_time.ReadSec();

	frame_time.Start();
}


// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	if (cap_FPS == false)
		cap_FPS = true;
	else
		cap_FPS = false;


	if (cap_FPS == true)
	{
		framerete_ms = 1000 / framerete_cap;
		cap = "ON";
	}
	else
	{
		framerete_ms = frame_time.Read();
		cap = "OFF";
	}
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
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	float last_frame_ms = frame_time.Read();
	//uint frames_on_last_update = prev_last_sec_frame_count;
	
	float init_sleep_timer = App->ptimer.ReadMs();

	if (last_frame_ms < framerete_ms && cap == "ON")
	{
		SDL_Delay(framerete_ms - last_frame_ms);

		float final_sleep_timer = App->ptimer.ReadMs();
		float sleep_timer = final_sleep_timer - init_sleep_timer;
	}


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
	this->framerete_cap = max_framerate;
}

uint Application::GetMaxFramerate() const
{
	return framerete_cap;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

float Application::GetFPS()
{
	return float(prev_last_sec_frame_count);
}

float Application::GetMS()
{
	return float(1000 / prev_last_sec_frame_count);
}

float Application::GetDt() const
{
	return dt;
}
