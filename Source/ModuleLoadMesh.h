#ifndef __MODULE_LOADING_MESH_H__
#define __MODULE_LOADING_MESH_H__

#include "Module.h"
#include "Globals.h"
#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

struct mesh_data
{
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertex = 0;
	float* vertex = nullptr;
	GLsizeiptr v_size = 0;

	uint num_normal = 0;
	float* normal = nullptr;
	GLsizeiptr n_size = 0;

	uint num_color = 0;
	float* color = nullptr;
	GLsizeiptr c_size = 0;

	uint num_textcoord = 0;
	float* textcoord = nullptr;
	GLsizeiptr t_size = 0;
};


class ModuleLoadMesh : public Module
{
public:
	ModuleLoadMesh(Application* app, bool start_enabled = true);
	~ModuleLoadMesh();

	bool Init();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void LoadMesh(char* path);
	void DrawMesh();

public:
	mesh_data fbx;

};




#endif // __MODULE_LOADING_MESH_H
