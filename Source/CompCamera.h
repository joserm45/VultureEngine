#ifndef __COMP_CAMERA_H__
#define __COMP_CAMERA_H__
#include "Globals.h"
#include "Components.h"
#include "MathGeoLib/include/MathGeoLib.h"

class CompCamera : public Components
{
public:

	CompCamera(GameObject* parent);
	~CompCamera();

	void SetAspectRatio(float ratio);
	void Draw();
	void Transform();

private:
	math::Frustum frustum;
	bool projection_changed = false;
	float fov = 60.0f;
	float aspect_ratio = 0.0f;

	math::float4x4 view_matrix = math::float4x4::zero;
	math::float4x4 projection_matrix = math::float4x4::zero;
};

#endif