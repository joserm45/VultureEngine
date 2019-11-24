#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "CompCamera.h"
#include "CompMesh.h"
#include "Quadtree.h"

#include "MathGeoLib\include\Math\MathAll.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	editor_camera = new CompCamera(nullptr);
	editor_camera->SetNearPlane(0.6f);
	editor_camera->SetFarPlane(512.0f);
	curr_camera = editor_camera;


	name = "Camera";

	CalculateViewMatrix();

	Move({ 0.0f, 2.0f, -8.0f });
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{

	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!
	if (following == nullptr)
	{
		float3 newPos(0, 0, 0);

		float speed = 8.0f * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed *= 2.0f;
		
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)newPos.y += speed;

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
		{
			FocusInObject();
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += curr_camera->frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= curr_camera->frustum.front *speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= curr_camera->frustum.WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)newPos += curr_camera->frustum.WorldRight() * speed;

		if (App->input->GetMouseZ()) newPos += curr_camera->frustum.front * speed * App->input->GetMouseZ();

		Move(newPos);

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
			if (!App->imgui->IsMouseHoveringWindow()) //If any ImGui window is being pressed, don't check picking
				CheckForMousePicking();
		}
		// Quadtree
		/*int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		if (dx != 0)
		{
			//Rotate arround the Y axis to rotate in the X coord axis
			Quat quat = Quat::RotateY(dx*dt*Sensitivity);
			curr_camera->frustum.up = quat.Mul(curr_camera->frustum.up).Normalized();
			curr_camera->frustum.front = quat.Mul(curr_camera->frustum.front).Normalized();
		}

		if (dy != 0)
		{
			//Rotate arround the X local axis to rotate in the Y coord axis
			Quat quat = Quat::RotateAxisAngle(curr_camera->frustum.WorldRight(), dy*dt*Sensitivity);
			float3 up = quat.Mul(curr_camera->frustum.up).Normalized();
			//Cap that you can be upside down in engine
			if (up.y > 0.0f) {
				curr_camera->frustum.up = up;
				curr_camera->frustum.front = quat.Mul(curr_camera->frustum.front).Normalized();
			}
		}*/
		// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			if (dx != 0)
			{
				//Rotate arround the Y axis to rotate in the X coord axis
				Quat quat = Quat::RotateY(dx*dt*Sensitivity);
				curr_camera->frustum.up = quat.Mul(curr_camera->frustum.up).Normalized();
				curr_camera->frustum.front = quat.Mul(curr_camera->frustum.front).Normalized();
			}

			if (dy != 0)
			{
				//Rotate arround the X local axis to rotate in the Y coord axis
				Quat quat = Quat::RotateAxisAngle(curr_camera->frustum.WorldRight(), dy*dt*Sensitivity);
				float3 up = quat.Mul(curr_camera->frustum.up).Normalized();
				//Cap that you can be upside down in engine
				if (up.y > 0.0f) 
				{
					curr_camera->frustum.up = up;
					curr_camera->frustum.front = quat.Mul(curr_camera->frustum.front).Normalized();
				}
			}
		}
	}

	if ((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RALT) == KEY_REPEAT) &&
		App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.20f;
		
		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
		LookAt(X);
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference)
{

	curr_camera->frustum.pos = Position;
	LookAt(Reference);

}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

void ModuleCamera3D::LookAt(const float3 &Spot)
{
	float3 dir = Spot - curr_camera->frustum.pos;
	dir.Normalize();
	float3x3 mat = float3x3::LookAt(curr_camera->frustum.front, dir, curr_camera->frustum.up, float3::unitY);
	curr_camera->frustum.front = mat.MulDir(curr_camera->frustum.front).Normalized();
	curr_camera->frustum.up = mat.MulDir(curr_camera->frustum.up).Normalized();
}

void ModuleCamera3D::LookAt(const math::float3& reference, float radius) const
{
	math::float3 Z = -(frustum.pos - reference).Normalized(); // Direction the camera is looking at (reverse direction of what the camera is targeting)
	math::float3 X = math::Cross(math::float3(0.0f, 1.0f, 0.0f), Z).Normalized(); // X is perpendicular to vectors Y and Z
	math::float3 Y = math::Cross(Z, X); // Y is perpendicular to vectors Z and X

	(float3)curr_camera->frustum.front = Z;
	(float3)curr_camera->frustum.up = Y;

	if (radius != 0.0f)
	{
		float distance = (curr_camera->frustum.pos - reference).Length();
		distance -= radius;

		//frustum.Translate(frustum.front * distance);
	}
}

void ModuleCamera3D::LookAround(const math::float3 & reference, float pitch, float yaw) const
{
	math::Quat rotationX = math::Quat::RotateAxisAngle({ 0.0f,1.0f,0.0f }, yaw * DEGTORAD);
	math::Quat rotationY = math::Quat::RotateAxisAngle(frustum.WorldRight(), pitch * DEGTORAD);

	math::Quat finalRotation = rotationX * rotationY;

	//frustum.up = finalRotation * frustum.up;
	//frustum.front = finalRotation * frustum.front;

	float distance = (frustum.pos - reference).Length();
	//frustum.pos = reference + (-frustum.front * distance);
}

void ModuleCamera3D::FocusInObject()
{
	GameObject* focus = App->scene_intro->GetFocusedGameObject();
	if (focus != nullptr && focus->mesh != nullptr)
		focus->CenterGameObject();
}

// -----------------------------------------------------------------
/*void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}*/

void ModuleCamera3D::Move(const float3 &Movement)
{
	curr_camera->frustum.Translate(Movement);
}

void ModuleCamera3D::CheckForMousePicking()
{
	vector<GameObject*> intersected_objs;

	//Get the screen coords and normalize it
	int mouse_pos_x = App->input->GetMouseX();
	int mouse_pos_y = App->input->GetMouseY();
	int scr_width, scr_height;

	App->window->GetWinSize(scr_width, scr_height);

	//Normalize 
	float norm_x = -(1.0f - 2.0f * ((float)mouse_pos_x) / ((float)scr_width));
	float norm_y = 1.0f - (2.0f * ((float)mouse_pos_y) / ((float)scr_height));

	LineSegment picking = editor_camera->frustum.UnProjectLineSegment(norm_x, norm_y);

	//Quadtree picking static intersected objects only looking in the TreeNode 
	App->scene_intro->quadtree->Intersect(intersected_objs, picking);

	//Get the AABB intersection of the dynamic objects
	GameObject* root_obj = App->scene_intro->GetRootGameObject();
	for (int i = 0; i < root_obj->GetNumChilds(); ++i)
		AABBIntersect(picking, root_obj->childs[i], intersected_objs);

	//We have the whole vector of intersections. Test which
	GameObject* nearest_GO = nullptr;
	GetAABBClosestObject(picking, intersected_objs, nearest_GO);

	if (nearest_GO) { //If there's something to pick
		if (nearest_GO->mesh->IsPrimitive()) //If the nearest GO AABB is a primitive automaticaly focus
			App->scene_intro->FocusGameObject(nearest_GO, App->scene_intro->GetRootGameObject());
		else
		{
			//Test the triangles of every mesh that isn't a primitive
			if (InsidePicking(picking, intersected_objs, nearest_GO))
				App->scene_intro->FocusGameObject(nearest_GO, App->scene_intro->GetRootGameObject());
			else
				int i = 0;
				//App->scene_intro->UnfocusGameObjects();
		}
	}
	else{}
		//App->scene_intro->UnfocusGameObjects();

}

void ModuleCamera3D::GetAABBClosestObject(LineSegment ray, std::vector<GameObject*> intersected_objs, GameObject*& nearest)
{
	float min_dist = FLOAT_INF;

	for (int i = 0; i < intersected_objs.size(); ++i) {
		float hit_dist = intersected_objs[i]->BBox.Distance(ray.a);
		if (hit_dist < min_dist) {
			nearest = intersected_objs[i];
			min_dist = hit_dist;
		}
	}
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::AABBIntersect(LineSegment picking, GameObject* inters_GO, std::vector<GameObject*>& intersected_objs)
{
	//Look for AABB intersections. Look for all the intersected object and the closest object to the ray
	if (inters_GO->BBox.IsFinite() && !inters_GO->IsStatic()) {
		if (picking.Intersects(inters_GO->BBox))
			intersected_objs.push_back(inters_GO);
	}

	for (int i = 0; i < inters_GO->GetNumChilds(); ++i)
		AABBIntersect(picking, inters_GO->childs[i], intersected_objs);
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

bool ModuleCamera3D::InsidePicking(LineSegment ray, vector<GameObject*> intersected_objs, GameObject*& nearest)
{
	bool ret = false;
	//set far plane as first distance check
	float nearest_distance = 10000.0f;
	float min_hit_dist = FLOAT_INF;

	for (int i = 0; i < intersected_objs.size(); ++i)
	{

		if (intersected_objs[i]->mesh->IsPrimitive()) //If we got a primitive don't check the triangles
			continue;

		//Pass ray to local coordinates
		LineSegment local_ray = ray;
		local_ray.Transform(intersected_objs[i]->GetGlobalMatrix().Inverted());
		//static
		//App->scene_intro->quadtree.CollectIntersections(collided, my_ray);

		//check every mesh triangle
		for (int i = 0; i < intersected_objs.size(); i++)
		{
			math::LineSegment local_ray = ray;
			local_ray.Transform(intersected_objs[i]->GetGlobalMatrix().Inverted());

			CompMesh* mesh = (CompMesh*)intersected_objs[i]->mesh;

			for (int j = 0; j < mesh->mesh_info.num_index; j++)
			{
				math::Triangle triangle;

				triangle.a = math::float3(mesh->mesh_info.vertex[j * 3], mesh->mesh_info.vertex[j * 3 + 1], mesh->mesh_info.vertex[j * 3 + 2]);
				j++;
				triangle.b = math::float3(mesh->mesh_info.vertex[j * 3], mesh->mesh_info.vertex[j * 3 + 1], mesh->mesh_info.vertex[j * 3 + 2]);
				j++;
				triangle.c = math::float3(mesh->mesh_info.vertex[j * 3], mesh->mesh_info.vertex[j * 3 + 1], mesh->mesh_info.vertex[j * 3 + 2]);

				float distance = 10000.0f;
				math::float3 hit_point;

				if (local_ray.Intersects(triangle, &distance, &hit_point)) {
					ret = true;
					if (distance < min_hit_dist) {
						nearest = intersected_objs[i];
						min_hit_dist = distance;
					}
				}
			}
		}
	}
	return ret;
}

//FollowCamera
void ModuleCamera3D::SelectFollowItem(PhysBody3D* body, float min, float max, float height)
{
	min_following_dist = min;
	max_following_dist = max;
	following_height = height;
	following = body;
}

void ModuleCamera3D::Follow()
{
	mat4x4 m;
	//following->GetTransform(&m);

	Look(Position, m.translation(), true);

	// Correct height
	Position.y = m[13] + 7;
	//Position.y = (14.9*Position.y + Position.y + following_height) / 16.0;

	// Correct distance
	vec3 cam_to_target = m.translation() - Position;
	float dist = length(cam_to_target);
	float correctionFactor = 0.f;
	if (dist < min_following_dist)
	{
		correctionFactor = 0.15*(min_following_dist - dist) / dist;
	}
	if (dist > max_following_dist)
	{
		correctionFactor = 0.15*(max_following_dist - dist) / dist;
	}
	Position -= correctionFactor * cam_to_target;
}

