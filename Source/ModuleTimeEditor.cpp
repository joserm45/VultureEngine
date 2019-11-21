#include "Application.h"
#include "Globals.h"
#include "ModuleTimeEditor.h"



ModuleTimeEditor::ModuleTimeEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Time Editor";
}


ModuleTimeEditor::~ModuleTimeEditor()
{
}

update_status ModuleTimeEditor::PostUpdate(float dt)
{
	frame_count++;

	app_dt = App->GetDt();
	app_time += app_dt;

	switch (App->GetState()) {
	case ENGINE_STATE_EDITOR:
		break;

	case ENGINE_STATE_PLAY:
		this->dt = app_dt * time_scale;
		time += this->dt;
		break;

	case ENGINE_STATE_PAUSE:
		this->dt = 0.0f;
		break;

	}

	return UPDATE_CONTINUE;
}

bool ModuleTimeEditor::CleanUp()
{
	return true;
}

void ModuleTimeEditor::ResetGameTimer()
{
	time = 0.0f;
	dt = 0.0f;
}

uint ModuleTimeEditor::GetFrameCount() const
{
	return frame_count;
}

float ModuleTimeEditor::GetAppTimeClock() const
{
	return app_time;
}

float ModuleTimeEditor::GetAppdt() const
{
	return app_dt;
}

float ModuleTimeEditor::GetGameTime() const
{
	return time;
}

float ModuleTimeEditor::GetTimeScale() const
{
	return time_scale;
}

float ModuleTimeEditor::GetGamedt() const
{
	return dt;
}
