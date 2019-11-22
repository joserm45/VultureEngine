#ifndef __COMPTRANSFORM_H__
#define __COMPTRANSFORM_H__

#include "Components.h"

#include "MathGeoLib/include/MathGeoLib.h"


class CompTransform : public Components
{
public:

	CompTransform(GameObject* parent);
	~CompTransform();

	void Update();
	math::float3 GetPosition();
	void SetPosition(math::float3 pos);

	math::Quat GetRotation();
	void SetRotation(math::Quat rot);

	math::float3 GetScale();
	void SetScale(math::float3 scale);

	math::float4x4 GetGlobalMatrix() const;
	math::float3 GetGlobalPos() const;
	void Draw();


public:

	math::float4x4 global_matrix = math::float4x4::identity;
	math::float4x4 local_matrix = math::float4x4::identity;

	math::float3 position = math::float3::zero;
	math::Quat rotation = math::Quat::identity;
	math::float3 scale = math::float3::one;	

	math::float3 rotate = math::float3::zero;
};
#endif