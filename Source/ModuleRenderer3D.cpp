#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleImGui.h"
#include "Quadtree.h"
#include "CompCamera.h"
#include "CompTransform.h"
#include "Components.h"
#include "CompMesh.h"
#include "CompMaterial.h"

#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#include "Imgui\imgui.h"
#include "imgui\imgui_impl_sdl.h"
#include "imgui\imgui_impl_opengl3.h"

#include <stdio.h>

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment(lib, "glew/libx86/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Render";

	shaders_manager = new ShaderProgramManager();
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context

	//TODO ImGui::CreateContext();

	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		// Initialize glew
		GLenum error_glew = glewInit();
		if (error_glew != GL_NO_ERROR)
		{
			LOG("Error initializing glew! %s\n", glewGetErrorString(error_glew));
			ret = false;
		}
		else
		{
			LOG("Status: Using GLEW! %s \n", glewGetString(GLEW_VERSION));
		}

		// Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		// Check for error
		error_glew = glGetError();
		if (error_glew != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error_glew));
			ret = false;
		}

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		//glClearColor(0.53f, 0.81f, 1.f, 1.f); //135, 206,255 - Blue

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		lights[0].Active(true);

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	GLubyte defaultImage[128][128][4];

	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < 128; j++) {

			defaultImage[i][j][0] = (GLubyte)255;
			defaultImage[i][j][1] = (GLubyte)255;
			defaultImage[i][j][2] = (GLubyte)255;
			defaultImage[i][j][3] = (GLubyte)255;
		}
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &defTexture);
	glBindTexture(GL_TEXTURE_2D, defTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128,
		0, GL_RGBA, GL_UNSIGNED_BYTE, defaultImage);

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//default shader
	shaders_manager->default_shader = * shaders_manager->CreateDefaultShaderProgram();
	shaders_manager->water_shader = * shaders_manager->CreateWaterShaderProgram();
	//defTexture = App->importer->LoadTextureData("background");
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	//ImGui::NewFrame();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->curr_camera->GetViewMatrix());

	UpdateGameObjectMatrix(App->scene_intro->GetRootGameObject());

	// light 0 on cam pos
	float3 cam_pos = App->camera->curr_camera->frustum.pos;
	lights[0].SetPos(cam_pos.x, cam_pos.y, cam_pos.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	if (grid)
	{
		glBegin(GL_LINES);
		glColor3f(255, 255, 255);
		for (int i = -20; i <= 20; i++) {

			glVertex3f(-20, 0, i);
			glVertex3f(20, 0, i);

			glVertex3f(i, 0, -20);
			glVertex3f(i, 0, 20);

		}
		glEnd();
	}
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL2_NewFrame(App->window->window);
	//ImGui::NewFrame();

	App->scene_intro->Draw();
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{

	GameObject* go = nullptr;
	App->imgui->Draw();

	SDL_GL_SwapWindow(App->window->window);

	// Shader Draw
	//go = App->scene_intro->GetRootGameObject();
	/*uint l = 0;
	for (std::vector<GameObject*>::const_iterator i = App->scene_intro->GO_list.begin(); i != App->scene_intro->GO_list.end(); ++i)
	{
		
		UpdateShader(i[l]->mesh);
		l++;
	}

	/*if (go->GetNumChilds() > 0) 
	{
		for (int i = 0; i < go->GetNumChilds(); i++) {
			if (go->GetChild(i)->material != nullptr)
				UpdateShader(go->GetChild(i)->mesh);
		}
	}*/


	return UPDATE_CONTINUE;
}

void ModuleRenderer3D::UpdateShader(CompMesh * object)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ShaderProgram* shader_active = &shaders_manager->default_shader;
	shader_active->UseProgram();
	
	if (object != nullptr)
	{
		CompMaterial* texture = (CompMaterial*)object->GetGameObject()->FindComponent(MATERIAL);
		/*if (object->mesh_info.num_textcoord != 0 /*&& texture->IsAlphaTest())
			glEnable(GL_ALPHA_TEST);
		//glAlphaFunc(GL_GREATER, texture->GetAlphaValue());
		glBindTexture(GL_TEXTURE_2D, texture->tex_id);*/

		if (texture->default_shader)
		{
			shader_active = &shaders_manager->water_shader;
			if (shader_active != nullptr)
				shader_active->UseProgram();
		}
		else if (texture->own_shader)
		{
			shader_active = texture->GetShader();
			if (shader_active != nullptr)
				shader_active->UseProgram();
		}

		if (shader_active != nullptr)
		{
			glUniform1i(glGetUniformLocation(shader_active->id_shader_prog, "ourTexture"), 0);
			glUniform1f(glGetUniformLocation(shader_active->id_shader_prog, "ftime"), shaderDt);
		}

	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, defTexture);
		glUniform1i(glGetUniformLocation(shaders_manager->default_shader.id_shader_prog, "ourTexture"), 0);
	}
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::UpdateGameObjectMatrix(GameObject* game_object)
{
	if (game_object)
	{
		game_object->transform->UpdateMatrix();
		game_object->DrawBBox();

	}


	if (game_object) {
		for (int i = 0; i < game_object->GetNumChilds(); i++) {
			if (game_object->GetChild(i) != nullptr)
				UpdateGameObjectMatrix(game_object->GetChild(i));
		}
	}
}

void ModuleRenderer3D::RecalculateProjMat()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf(App->camera->curr_camera->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
