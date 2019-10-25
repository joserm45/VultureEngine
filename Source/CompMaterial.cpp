#include "CompMaterial.h"
#include "Imgui\imgui.h"

CompMaterial::CompMaterial(GameObject * parent, const char* path)
{
	type = MATERIAL;
	path_name = path;
}

CompMaterial::~CompMaterial()
{
}

void CompMaterial::Draw()
{
	if (ImGui::CollapsingHeader("Material")) {
		ImGui::Text("Texture path: ");
		ImGui::SameLine();

		if (texture_active) 
		{
			ImGui::TextColored(ImVec4(0.3f, 0.5f, 0.5f, 1.0f), path_name);

			ImGui::Text("Texture size: %i x %i", tex_width, tex_height);

		}
		else
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "null");

		ImGui::Separator();

		ImGui::Text("Options");

		ImGui::Checkbox("Draw Texture", &texture_active);

	}
}

uint CompMaterial::GetTexId() const
{
	return tex_id;
}
