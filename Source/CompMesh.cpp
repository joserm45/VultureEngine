#include "CompMesh.h"
#include "Globals.h"
#include "Imgui\imgui.h"
#include "ModuleImport.h"

CompMesh::CompMesh(GameObject* parent,const char* path, int num_mesh)
{
	type = MESH;
	path_name = path;
	mesh_info = App->importer->fbx;
	text_info = App->importer->texture;
}

CompMesh::~CompMesh()
{
	//RELEASE_ARRAY(mesh_info.vertex);
	//RELEASE_ARRAY(mesh_info.index);
}

void CompMesh::AssignMesh(char * path)
{

}

bool CompMesh::IsPrimitive() const
{
	if (is_primitive == Primitive_None)
		return false;

	return true;
}

void CompMesh::Draw()
{
	if (ImGui::CollapsingHeader("Mesh")) {
		ImGui::Text("Mesh path: ");

		ImGui::SameLine();

		if (path_name)
			ImGui::TextColored(ImVec4(0.3f, 0.5f, 0.5f, 1.0f), path_name);
		else
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "\0");

	}
}


void CompMesh::SetTexture(text_data tex)
{
	text_info = tex;
}
