#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "imgui.h"

#include <gl/GL.h>
#include <gl/GLU.h>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//App->physics->debug = true;

	//Music Level
	//PlaySceneMusic();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	//CUBE
	float v0[] = { 1.0f, 1.0f, 0.0f };
	float v1[] = { 0.0f, 1.0f, 0.0f };
	float v2[] = { 0.0f, 0.0f, 0.0f };
	float v3[] = { 1.0f, 0.0f, 0.0f };
	float v4[] = { 1.0f, 0.0f, 1.0f };
	float v5[] = { 1.0f, 1.0f, 1.0f };
	float v6[] = { 0.0f, 1.0f, 1.0f };
	float v7[] = { 0.0f, 0.0f, 1.0f };

	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 255);
	//Front
	glVertex3fv(v0);
	glVertex3fv(v1);
	glVertex3fv(v2);
	glVertex3fv(v2);
	glVertex3fv(v3);
	glVertex3fv(v0);
	//Right
	glVertex3fv(v0);
	glVertex3fv(v3);
	glVertex3fv(v4);
	glVertex3fv(v4);
	glVertex3fv(v5);
	glVertex3fv(v0);
	//Up
	glVertex3fv(v0);
	glVertex3fv(v5);
	glVertex3fv(v6);
	glVertex3fv(v6);
	glVertex3fv(v1);
	glVertex3fv(v0);
	//Back
	glVertex3fv(v7);
	glVertex3fv(v6);
	glVertex3fv(v5);
	glVertex3fv(v5);
	glVertex3fv(v4);
	glVertex3fv(v7);
	//Left
	glVertex3fv(v7);
	glVertex3fv(v2);
	glVertex3fv(v1);
	glVertex3fv(v1);
	glVertex3fv(v6);
	glVertex3fv(v7);
	//Down
	glVertex3fv(v7);
	glVertex3fv(v4);
	glVertex3fv(v3);
	glVertex3fv(v3);
	glVertex3fv(v2);
	glVertex3fv(v7);
	glEnd();

	return UPDATE_CONTINUE;
}





void ModuleSceneIntro::PlaySceneMusic()
{
	
}
