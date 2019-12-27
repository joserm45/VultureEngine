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
	//void SetTexture(text_data tex);

	//text_data text_info;	
	bool sample_shader = false;
	bool water_shader = false;
	bool own_shader = false;

	ShaderProgram* GetShader() const;

private:

	bool texture_active = true;
	bool chess_texture = false;

	const char* path_name;
	
	GameObject* parent = nullptr;

	//shaders
	ShaderProgram* shader = nullptr;
	bool combo_type = false;
	int shaders_type = 2;
};

#endif