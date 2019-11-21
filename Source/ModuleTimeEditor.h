#ifndef _MODULE_TIME_EDITOR_H__
#define _MODULE_TIME_EDITOR_H__

#include "Module.h"
#include "Globals.h"

class ModuleTimeEditor : public Module
{
public:
	ModuleTimeEditor(Application* app, bool start_enabled = true);
	~ModuleTimeEditor();

	update_status PostUpdate(float dt);
	bool CleanUp();

	void ResetGameTimer();

	uint GetFrameCount() const;
	float GetAppTimeClock() const;
	float GetAppdt() const;
	float GetGameTime() const;
	float GetTimeScale() const;
	float GetGamedt() const;

public:

	float time_scale = 1.0f; 

private:

	// App Time
	uint frame_count = 0; 
	float app_time = 0.0f; 
	float app_dt = 0.0f; 

	//Game clock
	float time = 0.0f; 
	float dt = 0.0f; 

};

#endif