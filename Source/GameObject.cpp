#include "Application.h"
#include "Globals.h"
#include "GameObject.h"
#include "Quadtree.h"

#include "Components.h"
#include "CompTransform.h"
#include "CompMaterial.h"
#include "CompMesh.h"
#include "CompCamera.h"

#include "imgui/imgui.h"
#include "ImGuizmo\ImGuizmo.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GameObject::GameObject()
{
}

GameObject::GameObject(GameObject* root)
{
	this->parent = root;
	name = "Game Object";
	if (root != nullptr)
	{
		root->childs.push_back(this);
	}


	CreateComponent(TRANSFORM,0,"null");

	//shaders_manager = new ShaderProgramManager();
}


GameObject::~GameObject()
{
	for (int i = 0; i < childs.size(); i++)
		RELEASE(childs[i]);
	childs.clear();

	RELEASE(transform);	//CLEAN ALL COMPONENTS
	RELEASE(mesh);
	RELEASE(material); //crash
	RELEASE(camera);

	parent = nullptr;
}

void GameObject::Update()
{
	shader_dt = App->ptimer.ReadMs();
	for (uint i = 0; i < childs.size(); i++) 
	{
		childs[i]->Update();
	}

	if (transform && this != App->scene_intro->GetRootGameObject())
		transform->Update();

	if (camera)
		camera->Update();
}

Components* GameObject::CreateComponent(TYPECOMP type, int num_mesh, const char* path )
{
	Components* new_component = nullptr;

	switch (type) 
	{
		case TRANSFORM:
			if (transform == nullptr) 
			{
				transform = new CompTransform(this);
				new_component = transform;
				//new_component.push_back(transform); 
				//component->SetGameObject(this);
			}
			break;
		
		case MATERIAL:
			if (material == nullptr) 
			{
				material = new CompMaterial(this, path);
				new_component = material;
				//new_component.push_back(material);
				//component->SetGameObject(this);
			}
			break;

		case MESH:
			if (mesh == nullptr) 
			{
				mesh = new CompMesh(this, path, num_mesh);
				new_component = mesh;
				//new_component.push_back(mesh);
				//component->SetGameObject(this);
				CreateBBox();
			}
			break;

		case CAMERA:
			if (camera == nullptr) 
			{
				new_component = camera;
				camera = new CompCamera(this);
				//new_component.push_back(camera);
				//component->SetGameObject(this);
			}
			break;

		case DEFAULT:
			return nullptr;
			break;
		}
	return new_component;
}

bool GameObject::IsVisible() const
{
	return active;
}

void GameObject::SetVisibility(bool active)
{
	this->active = active;
}


const char* GameObject::GetName()
{
	return name;
}

void GameObject::SetName(const char* new_name)
{
	name = new_name;
}

GameObject* GameObject::GetParent()
{
	return parent;
}

GameObject* GameObject::GetChild(uint number_child)
{
	return childs[number_child];
}

uint GameObject::GetNumChilds() const
{
	return childs.size();
}

bool GameObject::IsStatic() const
{
	return game_object_static;
}

void GameObject::PrintPanelGameObject(int& i, bool& clicked) {
	//Pop ID for each tree node
	ImGui::PushID(i);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	
	if (GetNumChilds() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (focused)
		flags |= ImGuiTreeNodeFlags_Selected;

	if (ImGui::TreeNodeEx((void*)(intptr_t)i, flags, name)) 
	{
		if (ImGui::IsItemClicked() && !clicked) 
		{
			App->imgui->AddLogToConsole("Node clicked");
			clicked = true;
			App->scene_intro->GetRootGameObject()->focused = false;
			App->scene_intro->FocusGameObject(this, App->scene_intro->GetRootGameObject()); //Focus
		}

		/*if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			GameObject* go = this;
			ImGui::SetDragDropPayload("Scene Game Object", &go, sizeof(GameObject));

			ImGui::EndDragDropSource();
		}*/
		//Print each child of the gameobject

		for (int j = 0; j < GetNumChilds(); j++) 
		{
			i++;
			GetChild(j)->PrintPanelGameObject(i, clicked);

		}

		ImGui::TreePop();
	}
	//Focus
	if (!clicked && ImGui::IsItemClicked()) 
	{
		LOG("%s node clicked", name);
		clicked = true;
		App->scene_intro->GetRootGameObject()->focused = false;
		App->scene_intro->FocusGameObject(this, App->scene_intro->GetRootGameObject());
	}
	ImGui::PopID();
}

void GameObject::DrawInspector()
{
	if (ImGui::CollapsingHeader("GameObject")) 
	{
		ImGui::PushID(0);
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.7f, 0.8f, 0.0f, 1.0f), name);
		ImGui::PopID();
	}

	if (ImGui::Checkbox("Static", &game_object_static)) 
	{
		if (game_object_static == true) {
			App->scene_intro->quadtree->Insert(this);
			App->scene_intro->statics_game_objects.push_back(this);
			App->scene_intro->CheckIfRebuildQuadtree(this);
		}
		else {
			App->scene_intro->quadtree->Remove(this);
			App->scene_intro->EraseObjFromStatic(this);
		}
	}

	if (transform != NULL) {
		ImGui::PushID(1);
		transform->Draw();
		ImGui::PopID();
	}

	if (mesh != NULL) {
		ImGui::PushID(2);
		mesh->Draw();
		ImGui::PopID();
	}

	if (material != NULL) {
		ImGui::PushID(3);
		material->Draw();
		ImGui::PopID();
	}

	if (camera != NULL) {
		ImGui::PushID(4);
		camera->Draw();
		ImGui::PopID();
	}
}

math::float4x4& GameObject::GetLocalMatrix()
{
	return float4x4::FromTRS(transform->position, transform->rotation, transform->scale);
}
math::float4x4 GameObject::GetGlobalMatrix()
{
	//float4x4 local_matrix = GetLocalMatrix();
	//local_matrix = transform->GetGlobalMatrix() * local_matrix;
	//return local_matrix;
	
	float4x4 local_matrix = GetLocalMatrix();
	if (GetParent()->GetParent() != nullptr)
	{
		float4x4 global_matrix = float4x4::identity;
		global_matrix = GetParent()->GetParent()->transform->GetGlobalMatrix();
		return global_matrix * local_matrix;
	}
	return local_matrix;
}
void GameObject::SetPosition(float3 position)
{
	transform->position = position;
	for (uint i = 0; i < childs.size(); ++i) {
		childs[i]->transform->position = position;
	}
	camera->Transform();
}
void GameObject::SetRotation(float3 rotation)
{
	transform->rotation = Quat::FromEulerXYZ(rotation.x * DEGTORAD, rotation.y * DEGTORAD, rotation.z *DEGTORAD);
	for (uint i = 0; i < childs.size(); ++i) {
		childs[i]->transform->rotation = Quat::FromEulerXYZ(rotation.x * DEGTORAD, rotation.y * DEGTORAD, rotation.z *DEGTORAD);
	}
	camera->Transform();
}

void GameObject::SetScale(float3 scale)
{
	transform->scale = scale;
	for (uint i = 0; i < childs.size(); ++i) 
	{
		childs[i]->transform->scale = scale;
	}
}

void GameObject::CenterGameObject() const 
{

	float3 dir_vector = -transform->GetGlobalPos() + App->camera->editor_camera->frustum.pos;
	dir_vector.Normalize();

	float rad = BBox.MinimalEnclosingSphere().r;
	float dist = math::Abs(App->camera->editor_camera->frustum.orthographicWidth / App->camera->editor_camera->frustum.orthographicHeight * rad / math::Sin(App->camera->editor_camera->frustum.verticalFov / 2));


	App->camera->Look(BBox.Centroid() + dir_vector * dist, BBox.Centroid());

}



void GameObject::CreateBBox()
{
	BBox.SetNegativeInfinity();
	BBox.Enclose((math::float3*)mesh->mesh_info.vertex, mesh->mesh_info.num_vertex);
	//we create the obb if we transform the gameobject
	math::OBB obb;
	obb.SetFrom(BBox);
	if (transform != nullptr)
	{
		obb.Transform(GetGlobalMatrix());
	}
	BBox = obb.MinimalEnclosingAABB();
	for (uint i = 0; i < childs.size(); ++i) {
		childs[i]->CreateBBox();
	}
}

void GameObject::DrawBBox()
{
	glBegin(GL_LINES);
	glLineWidth(1.0f);

	for (uint i = 0; i < BBox.NumEdges(); i++)
	{
		glVertex3f(BBox.Edge(i).a.x, BBox.Edge(i).a.y, BBox.Edge(i).a.z);
		glVertex3f(BBox.Edge(i).b.x, BBox.Edge(i).b.y, BBox.Edge(i).b.z);
	}
	glEnd();
	for (uint i = 0; i < childs.size(); ++i)
	{
		childs[i]->DrawBBox();
	}
}

void GameObject::DrawShaders()
{
	for (std::vector<GameObject*>::iterator i = App->scene_intro->GO_list.begin(); i != App->scene_intro->GO_list.end(); ++i)
	{
		if ((*i)->IsVisible() == true && (*i)->material->default_shader == true)
		{
			if (material != NULL)
			{
				math::float4x4 matrixfloat = transform->GetGlobalMatrix();
				GLfloat matrix[16] =
				{
					matrixfloat[0][0],matrixfloat[1][0],matrixfloat[2][0],matrixfloat[3][0],
					matrixfloat[0][1],matrixfloat[1][1],matrixfloat[2][1],matrixfloat[3][1],
					matrixfloat[0][2],matrixfloat[1][2],matrixfloat[2][2],matrixfloat[3][2],
					matrixfloat[0][3],matrixfloat[1][3],matrixfloat[2][3],matrixfloat[3][3]
				};

				uint offset = sizeof(float)* (3 + 3 + 4 + 2);
				if (mesh->mesh_info.textcoord == nullptr)
					offset -= sizeof(float) * 2;
				if (mesh->mesh_info.normal == nullptr)
					offset -= sizeof(float) * 3;

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				ShaderProgram* shader_active = &App->renderer3D->shaders_manager->default_shader;
				shader_active->UseProgram();


				shader_active = &App->renderer3D->shaders_manager->water_shader;
				if (shader_active != nullptr)
					shader_active->UseProgram();

				if (shader_active != nullptr)
				{
					glUniform1i(glGetUniformLocation(shader_active->id_shader_prog, "ourTexture"), 0);
					glUniform1f(glGetUniformLocation(shader_active->id_shader_prog, "ftime"), shader_dt);
				}

				glBindBuffer(GL_ARRAY_BUFFER, mesh->mesh_info.id_vertex);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, offset, 0); //VBO

				if (mesh->mesh_info.normal != nullptr)
				{
					glEnableVertexAttribArray(1);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, offset, BUFFER_OFFSET(sizeof(float) * 3)); //normals, 12 bytes from start
				}

				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, offset, BUFFER_OFFSET(sizeof(float) *(3 + 3))); //color, 24 bytes from start

				if (mesh->mesh_info.textcoord != nullptr)
				{
					glEnableVertexAttribArray(3);
					glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, offset, BUFFER_OFFSET(sizeof(float) * (3 + 3 + 4))); //texcoords, 40 bytes from start
				}

				if (mesh->mesh_info.id_index != NULL && shader_active != nullptr)
				{
					GLint view2Loc = glGetUniformLocation(shader_active->id_shader_prog, "view_matrix");
					math::float4x4 temp = App->camera->GetOpenGLViewMatrix();
					glUniformMatrix4fv(view2Loc, 1, GL_FALSE, temp.ptr());

					GLint modelLoc = glGetUniformLocation(shader_active->id_shader_prog, "model_matrix");
					temp = transform->GetGlobalMatrix();
					glUniformMatrix4fv(modelLoc, 1, GL_TRUE, temp.ptr());

					GLint viewLoc = glGetUniformLocation(shader_active->id_shader_prog, "proj_matrix");
					temp = App->camera->GetOpenGLProjectionMatrix();
					glUniformMatrix4fv(viewLoc, 1, GL_FALSE, temp.ptr());


					if (mesh->mesh_info.id_index != NULL)
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mesh_info.id_index);

					glDrawElements(GL_TRIANGLES, mesh->mesh_info.num_index, GL_UNSIGNED_INT, NULL);
				}


				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_BLEND);

				glUseProgram(NULL);

			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, DefaultTexture);
				glUniform1i(glGetUniformLocation(App->renderer3D->shaders_manager->default_shader.id_shader_prog, "ourTexture"), 0);
			}
		}
		else if ((*i)->material->texture_active == true)
		{
			//App->importer->LoadTexture((*i)->material->path_copy);
		}
	}
}

void GameObject::UpdateBoundingBox() {

	BBox.SetNegativeInfinity();

	if (mesh) 
	{
		BBox.Enclose((math::float3*)mesh->mesh_info.vertex, mesh->mesh_info.num_vertex);
	}

	if (transform) 
	{
		obb.SetFrom(BBox);
		obb.Transform(transform->GetGlobalMatrix());
		if (obb.IsFinite()) {
			BBox = obb.MinimalEnclosingAABB();
		}
	}

	for (uint i = 0; i < childs.size(); ++i) {

		childs[i]->UpdateBoundingBox();
	}

}

void GameObject::Draw()
{
	if (camera == NULL)
	{
		glDisable(GL_LIGHTING);
		//testing transformation
		glPushMatrix();
		glMultMatrixf(GetGlobalMatrix().Transposed().ptr());

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D, mesh->text_info.texture);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->mesh_info.id_vertex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mesh_info.id_index);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->mesh_info.id_texture);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		if (mesh->mesh_info.par_shape == false)
		{
			glDrawElements(GL_TRIANGLES, mesh->mesh_info.num_index, GL_UNSIGNED_INT, NULL);
		}
		else if (mesh->mesh_info.par_shape == true)
		{
			glDrawElements(GL_TRIANGLES, mesh->mesh_info.num_index, GL_UNSIGNED_SHORT, NULL);
		}
		//glDisable(GL_LIGHTING);
		//DrawBBox();
		//glEnable(GL_LIGHTING);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, NULL);

		glPopMatrix();
		
		DrawShaders();

	}
}

void GameObject::LoadTextureSelected(int x)
{
	if (x == 1)
	{
		if (App->scene_intro->GetFocusedGameObject() == this)
		{
			if (material != NULL)
			{
				App->importer->LoadChessTexture(this);
			}
		}
	}
	else if (x == 2)
	{
		if (App->scene_intro->GetFocusedGameObject() == this)
		{
			if (material != NULL)
			{
				//App->shader_manager->water_shader = *App->shader_manager->CreateWaterShaderProgram();				
			}
		}
	}
	else if (x == 3)
	{
		if (App->scene_intro->GetFocusedGameObject() == this)
		{
			if (material != NULL)
			{
				//App->shader_manager->default_shader = *App->shader_manager->CreateDefaultShaderProgram();
				DrawShaders();
			}
		}
	}
}

Components *GameObject::FindComponent(TYPECOMP type) const
{
	for (int i = 0; i < new_component.size(); i++)
	{
		if (new_component[i]->GetType() == type)
		{
			return new_component[i];
		}
	}

	return nullptr;;
}
void GameObject::ShowGizmos()
{
	ImGuizmo::Enable(true);

	static ImGuizmo::OPERATION operation(ImGuizmo::TRANSLATE);
	bool needs_update = false;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		operation = ImGuizmo::TRANSLATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		operation = ImGuizmo::ROTATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		operation = ImGuizmo::SCALE;
	}

	float* viewMatrix = App->scene_intro->camera->camera->GetViewMatrix();
	float* projMatrix = App->scene_intro->camera->camera->GetProjectionMatrix();
	math::float4x4 transMatrix = this->GetGlobalMatrix().Transposed();

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	ImGuizmo::Manipulate(viewMatrix, projMatrix, operation, ImGuizmo::MODE::LOCAL, (float*)&transMatrix);
	
	if (ImGuizmo::IsUsing())
	{
		
		transMatrix.Transpose();

		if (parent == App->scene_intro->GetRootGameObject())
		{
			transform->local_matrix = transMatrix;
		}
		else
		{
			transform->local_matrix = parent->GetGlobalMatrix().Inverted() * transMatrix;
		}
	
		transform->local_matrix.Decompose(transform->position, transform->rotation, transform->scale);
		transform->rotate = transform->rotation.ToEulerXYZ() * RADTODEG;
		needs_update = true;
	}

	if (needs_update)
	{
		//transform->local_matrix = math::float4x4::FromTRS(transform->position, transform->rotation, transform->scale);

		//if (parent != nullptr && parent != App->scene_intro->GetRootGameObject())
		//{
		//	transform->global_matrix = parent->GetGlobalMatrix() * transform->local_matrix;
		//}
		//else
		//{
		//	transform->global_matrix = transform->local_matrix;
		//}

		
		needs_update = false;
	}
}