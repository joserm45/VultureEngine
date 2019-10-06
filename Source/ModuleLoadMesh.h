#ifndef __MODULE_LOADING_MESH_H__
#define __MODULE_LOADING_MESH_H__

#include "Module.h"
#include "Globals.h"

struct mesh_data
{
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertex = 0;
	float* vertex = nullptr;
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

public:
	mesh_data fbx;

};




#endif // __MODULE_LOADING_MESH_H
