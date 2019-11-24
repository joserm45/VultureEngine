#pragma once
#include "Module.h"
#include "Globals.h"
#include "Components.h"
#include "glmath.h"

#include "MathGeoLib\include\Math\float3.h"
#include "MathGeoLib\include\Geometry\Frustum.h"
#include "MathGeoLib\include\Geometry\AABB.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void Look(const float3 &Position, const float3 &Reference);
	void LookAt(const vec3 &Spot);
	void LookAt(const math::float3& reference, float radius) const;
	void LookAt(const float3 &Spot);
	void LookAround(const math::float3& reference, float pitch, float yaw) const;
	
	void FocusInObject();
	//void Move(const vec3 &Movement);
	void Move(const float3 &Movement);
	void CheckForMousePicking();
	float* GetViewMatrix();

	void AABBIntersect(LineSegment picking, GameObject* inters_GO, std::vector<GameObject*>& intersected_objs);
	void GetAABBClosestObject(LineSegment ray, std::vector<GameObject*> intersected_objs, GameObject*& nearest);

	//Follow a body
	void SelectFollowItem(PhysBody3D* body, float min, float max, float height);
	void Follow();

private:

	void CalculateViewMatrix();
	bool InsidePicking(LineSegment ray, std::vector<GameObject*> intersected_objs, GameObject*& nearest);

public:
	
	vec3 X, Y, Z, Position, Reference;
	math::Frustum frustum;
	math::float3 reference = { 0.0f,0.0f,0.0f };

	CompCamera* curr_camera = nullptr;
	CompCamera* editor_camera = nullptr;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;

	//CameraFollow
	PhysBody3D* following = nullptr;
	float min_following_dist;
	float max_following_dist;
	float following_height;

};