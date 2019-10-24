#ifndef __MODULE_LOADING_MESH_H__
#define __MODULE_LOADING_MESH_H__

#include "Module.h"
#include "Globals.h"
#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Par\par_shapes.h"

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


class ModuleImport : public Module
{
public:
	ModuleImport(Application* app, bool start_enabled = true);
	~ModuleImport();

	bool Init();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void LoadMesh(char* path, bool is_parshape = false, uint i = 0);
	void DrawMesh(bool is_parshape);
	void ClearMeshData();

	//void LoadParShape(uint i);
	//void DrawParShape();

public:
	mesh_data fbx;
	bool par_shape = false;
	//mesh_data shape_data;
	par_shapes_mesh* shape;
};




#endif // __MODULE_LOADING_MESH_H
