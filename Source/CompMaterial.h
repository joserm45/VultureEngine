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
	char path_copy[100];
	uint tex_id = 0;
	int tex_width = 0, tex_height = 0;
private:

	bool texture_active = true;
	bool chess_texture = false;
	const char* path_name;
	
	

};

#endif