#ifndef __COMP_MESH_H__
#define __COMP_MESH_H__

#include "Components.h"
#include "Application.h"
#include "ModuleImport.h"


class ComponentMesh : public Components
{
public:

	ComponentMesh(GameObject* parent, char* path, int num_mesh);
	~ComponentMesh();

	void AssignMesh(char* path);

	bool IsPrimitive() const;

	void  DrawInspector();

	void FillPrimitiveDrawInfo(mesh_data info);

private:

	mesh_data mesh_info;
	const char* path = nullptr;
	int num_mesh = 0;
	int is_primitive = false;
};

#endif
