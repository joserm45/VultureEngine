#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"


struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void DrawCubeDirectMode();
	void DrawCubeVertexArrays();
	void DrawCubeDrawElements();

	void PlaySceneMusic();

	bool my_tool_active = true;
	
	char buf[10] = "casa";
	float f = 1.0f;
	//float my_color[] = { 0.0f, 0.0f, 1.0f };
};
