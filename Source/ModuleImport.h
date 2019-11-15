#ifndef __MODULE_LOADING_MESH_H__
#define __MODULE_LOADING_MESH_H__

#include "Module.h"
#include "Globals.h"
#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Par\par_shapes.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"
#include "DevIL\include\ilut.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#pragma comment (lib, "DevIL\\libx86\\DevIL.lib")
#pragma comment (lib, "DevIL\\libx86\\ILU.lib")
#pragma comment (lib, "DevIL\\libx86\\ILUT.lib")

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
	//texture
	uint id_texture = 0;
	uint widht = 0;
	uint height = 0;
	bool par_shape = false;
};

struct text_data 
{
	char name[100];
	ILuint texture = 0;
	uint widht = 0;
	uint height = 0;
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
	void LoadTexture(const char* path);
	void DrawMesh(bool is_parshape, mesh_data fbx);
	void ClearMeshData();
	void LoadChessTexture();
	text_data LoadTextureData(ILuint id, uint w, uint h);
	//void LoadParShape(uint i);
	//void DrawParShape();

public:
	mesh_data fbx;
	
	//mesh_data shape_data;
	par_shapes_mesh* shape;

	//texture
	list<mesh_data> mesh_list;
	text_data texture;

	//testing transformation
	GameObject* last_GO = nullptr;
};




#endif // __MODULE_LOADING_MESH_H
