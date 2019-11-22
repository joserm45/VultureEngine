#ifndef __COMP_CAMERA_H__
#define __COMP_CAMERA_H__
#include "Globals.h"
#include "Components.h"
#include "MathGeoLib/include/MathGeoLib.h"

enum Intersection {
	INSIDE = 0,
	OUTSIDE = 1
};

class CompCamera : public Components
{
public:

	CompCamera(GameObject* parent);
	~CompCamera();
	void Update();
	void SetAspectRatio(float ratio);
	void Draw();
	void DrawCamera();
	void Transform();

	void SetFov();
	void SetVerticalFOV(float value);
	void CameraCulling(GameObject* go);

	void UpdateMatrix();
	//camera view 
	float* GetViewMatrix()const;
	float* GetProjectionMatrix()const;
	math::Frustum GetFrustum() const;
	int CompCamera::InsideAABB(const AABB& bbox) const;

private:

	math::Frustum frustum;
	bool projection_changed = false;
	float fov = 60.0f;
	float aspect_ratio = 0.0f;
	math::float4x4 view_matrix = math::float4x4::zero;
	math::float4x4 projection_matrix = math::float4x4::zero;

public:
	bool camera_culling = false;
	bool camer_active = false;
};

#endif