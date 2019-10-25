#ifndef __COMP_MESH_H__
#define __COMP_MESH_H__

#include "Components.h"
#include "Application.h"
#include "ModuleImport.h"


class CompMesh : public Components
{
public:

	CompMesh(GameObject* parent,const char* path, int num_mesh);
	~CompMesh();

	void AssignMesh(char* path);
	bool IsPrimitive() const;
	void  Draw();

	void FillPrimitiveDrawInfo(mesh_data info);

private:

	mesh_data mesh_info;
	const char* path_name = nullptr;
	int num_mesh = 0;
	int is_primitive = false;
};

#endif
