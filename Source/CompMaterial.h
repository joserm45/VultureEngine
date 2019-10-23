#ifndef __COMP_MATIRIAL_H__
#define __COMP_MATIRIAL_H__
#include "Globals.h"
#include "Components.h"


class ComponentMaterial : public Components
{
public:

	ComponentMaterial(GameObject* parent);
	~ComponentMaterial();

	void AssignTexture(const char* path);
	uint GetTexId()const;
	void  DrawInspector();

private:

	const char* tex_path;
	uint tex_id = 0;
	int tex_width, tex_height;

};

#endif