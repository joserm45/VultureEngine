#include "Globals.h"
#include "CompTransform.h"
#include "GameObject.h"

#include "MathGeoLib/include/MathGeoLib.h"


CompTransform::CompTransform(GameObject* parent) : Components(parent)
{
	LOG("Added Transform to GO");
	type = TRANSFORM;
}


CompTransform::~CompTransform()
{
}

math::float3 CompTransform::GetPosition() {

	return position;
}

void CompTransform::SetPosition(math::float3 pos) {

	position = pos;
}

math::float3 CompTransform::GetRotation() {

	return rotation;
}

void CompTransform::SetRotation(math::float3 rot) {

	rotation = rot;
}

math::float3 CompTransform::GetScale() {

	return scale;
}

void CompTransform::SetScale(math::float3 scale) {

	this->scale = scale;
}

math::float4x4 CompTransform::GetGlobalMatrix() const {

	return global_matrix;
}

