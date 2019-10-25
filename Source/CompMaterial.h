#ifndef __COMP_MATIRIAL_H__
#define __COMP_MATIRIAL_H__
#include "Globals.h"
#include "Components.h"


class CompMaterial : public Components
{
public:

	CompMaterial(GameObject* parent, const char* path);
	~CompMaterial();

	void AssignTexture(const char* path);
	void Draw();
	uint GetTexId()const;

private:

	bool texture_active = false;
	const char* path_name;
	uint tex_id = 0;
	int tex_width, tex_height;

};

#endif