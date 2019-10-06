#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "imgui.h"
#include "ModuleInput.h"

#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
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
	if (App->input->keyboard[SDL_SCANCODE_M] == KEY_DOWN)
	{
		App->SaveProject();
	}
	if (App->input->keyboard[SDL_SCANCODE_N] == KEY_DOWN)
	{
		App->LoadProject();
	}
	
	//Draw Cubes
	//DrawCubeDirectMode();
	
	//DrawCubeVertexArrays();

	//DrawCubeDrawElements();
	

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::DrawCubeDirectMode()
{
	//CUBE 1 DIRECT MODE
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
}

void ModuleSceneIntro::DrawCubeVertexArrays()
{
	//CUBE 2 VERTEX ARRAYS & GLDRAWARRAYS
	float v0_VA[] = { 3.0f, 1.0f, 0.0f };
	float v1_VA[] = { 2.0f, 1.0f, 0.0f };
	float v2_VA[] = { 2.0f, 0.0f, 0.0f };
	float v3_VA[] = { 3.0f, 0.0f, 0.0f };
	float v4_VA[] = { 3.0f, 0.0f, 1.0f };
	float v5_VA[] = { 3.0f, 1.0f, 1.0f };
	float v6_VA[] = { 2.0f, 1.0f, 1.0f };
	float v7_VA[] = { 2.0f, 0.0f, 1.0f };

	float vertices[] = {
		//Front
			v0_VA[0], v0_VA[1], v0_VA[2],
			v1_VA[0], v1_VA[1], v1_VA[2],
			v2_VA[0], v2_VA[1], v2_VA[2],
			v2_VA[0], v2_VA[1], v2_VA[2],
			v3_VA[0], v3_VA[1], v3_VA[2],
			v0_VA[0], v0_VA[1], v0_VA[2],
		//Right
			v0_VA[0], v0_VA[1], v0_VA[2],
			v3_VA[0], v3_VA[1], v3_VA[2],
			v4_VA[0], v4_VA[1], v4_VA[2],
			v4_VA[0], v4_VA[1], v4_VA[2],
			v5_VA[0], v5_VA[1], v5_VA[2],
			v0_VA[0], v0_VA[1], v0_VA[2],
		//Up
			v0_VA[0], v0_VA[1], v0_VA[2],
			v5_VA[0], v5_VA[1], v5_VA[2],
			v6_VA[0], v6_VA[1], v6_VA[2],
			v6_VA[0], v6_VA[1], v6_VA[2],
			v1_VA[0], v1_VA[1], v1_VA[2],
			v0_VA[0], v0_VA[1], v0_VA[2],
		//Back
			v7_VA[0], v7_VA[1], v7_VA[2],
			v6_VA[0], v6_VA[1], v6_VA[2],
			v5_VA[0], v5_VA[1], v5_VA[2],
			v5_VA[0], v5_VA[1], v5_VA[2],
			v4_VA[0], v4_VA[1], v4_VA[2],
			v7_VA[0], v7_VA[1], v7_VA[2],
		//Left
			v7_VA[0], v7_VA[1], v7_VA[2],
			v2_VA[0], v2_VA[1], v2_VA[2],
			v1_VA[0], v1_VA[1], v1_VA[2],
			v1_VA[0], v1_VA[1], v1_VA[2],
			v6_VA[0], v6_VA[1], v6_VA[2],
			v7_VA[0], v7_VA[1], v7_VA[2],
		//Down
			v7_VA[0], v7_VA[1], v7_VA[2],
			v4_VA[0], v4_VA[1], v4_VA[2],
			v3_VA[0], v3_VA[1], v3_VA[2],
			v3_VA[0], v3_VA[1], v3_VA[2],
			v2_VA[0], v2_VA[1], v2_VA[2],
			v7_VA[0], v7_VA[1], v7_VA[2] };

	uint my_id = 0;
	glGenBuffers(1, (GLuint*)&(my_id));
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(255, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleSceneIntro::DrawCubeDrawElements()
{
	//CUBE 3 GLDRAWELEMENTS
	float vertices_DE[] = {
		5.0f, 1.0f, 0.0f, 
		4.0f, 1.0f, 0.0f,
		4.0f, 0.0f, 0.0f,
		5.0f, 0.0f, 0.0f,
		5.0f, 0.0f, 1.0f,
		5.0f, 1.0f, 1.0f,
		4.0f, 1.0f, 1.0f,
		4.0f, 0.0f, 1.0f
	};

	uint indices[] = {
		0, 1, 2,  2, 3, 0,
		0, 3, 4,  4, 5, 0,
		0, 5, 6,  6, 1, 0,
		7, 6, 5,  5, 4, 7,
		7, 2, 1,  1, 6, 7,
		7, 4, 3,  3, 2, 7
	};

	uint my_id2 = 0;
	uint my_indices = 0;
	glGenBuffers(1, (GLuint*)&(my_id2));
	glGenBuffers(1, (GLuint*)&(my_indices));
	glBindBuffer(GL_ARRAY_BUFFER, my_id2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeof(vertices_DE) * 3, vertices_DE, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(0, 255, 0);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleSceneIntro::PlaySceneMusic()
{
	
}
