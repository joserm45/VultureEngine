
#include "CompMesh.h"
#include "CompMaterial.h"
#include "Imgui\imgui.h"
#include "Application.h"

CompMaterial::CompMaterial(GameObject * parent, const char* path)
{
	type = MATERIAL;
	path_name = path;
	this->parent = parent;

	char* path_start = "Assets/";

	strcpy(path_copy, path_start);
	strcat(path_copy, path);

	App->importer->LoadTexture(path_copy);

	strcpy(App->importer->texture.name, path_copy);

	//shader = App->renderer3D->shaders_manager->CreateDefaultShaderProgram();
	//text_info = App->importer->texture;
}

CompMaterial::~CompMaterial()
{
}

void CompMaterial::Draw()
{
	strcpy(path_copy, App->importer->texture.name);
	tex_id = App->importer->texture.texture;
	tex_width = App->importer->texture.widht;
	tex_height = App->importer->texture.height;
	if (ImGui::CollapsingHeader("Material")) {
		ImGui::Text("Texture path: ");
		ImGui::SameLine();

		if (texture_active) 
		{
			ImGui::TextColored(ImVec4(0.3f, 0.5f, 0.5f, 1.0f), path_copy);

			ImGui::Text("Texture size: %i x %i", tex_width, tex_height);

		}
		else
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "null");

		ImGui::Separator();

		ImGui::Text("Options");

		if (ImGui::Checkbox("Draw Texture", &texture_active))
		{
			texture_active != texture_active;
			chess_texture = false;
			water_shader = false;
			own_shader = false;
			default_shader = false;
			
			if(texture_active)
				App->importer->LoadTexture(path_copy);
		}

		else if (ImGui::Checkbox("Default Shader", &default_shader))
		{
			default_shader != default_shader;
			texture_active = false;
			chess_texture = false;
			water_shader = false;
			own_shader = false;

			if(default_shader)
				parent->LoadTextureSelected(3);
		}

		else if (ImGui::Checkbox("Chess Texture", &chess_texture))
		{
			chess_texture != chess_texture;
			texture_active = false;
			water_shader = false;
			own_shader = false;
			default_shader = false;
			
			if(chess_texture)
				parent->LoadTextureSelected(1);
		}

		else if (ImGui::Checkbox("Water Shader", &water_shader))
		{
			water_shader != water_shader;
			texture_active = false;
			chess_texture = false;
			own_shader = false;
			default_shader = false;
			
			if(water_shader)
				parent->LoadTextureSelected(2);
		}
		else if (ImGui::Checkbox("Own Shader", &own_shader))
		{
			texture_active = false;
			chess_texture = false;
			water_shader = false;
			default_shader = false;
			
			if (ImGui::Button("Edit", ImVec2(100, 0)))
			{

			}
				
		}

	}
}

uint CompMaterial::GetTexId() const
{
	return tex_id;
}

ShaderProgram * CompMaterial::GetShader() const
{
	return shader;
}




//void CompMaterial::SetTexture(text_data tex)
//{
	//text_info = tex;
//}