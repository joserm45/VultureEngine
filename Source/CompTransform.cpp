#include "Globals.h"
#include "CompTransform.h"
#include "GameObject.h"
#include "Imgui\imgui.h"

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

math::Quat CompTransform::GetRotation() {

	return rotation;
}

void CompTransform::SetRotation(math::Quat rot) {

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

void CompTransform::Draw()
{
	if (ImGui::CollapsingHeader("Transform")) {

		ImGui::Text("Position");
		ImGui::SameLine();
		if(ImGui::DragFloat3("p", &position[0], 0.1f))
		{
			gameObject->SetPosition(position);
		}

		rotate = rotation.ToEulerXYZ() * RADTODEG;
		ImGui::Text("Rotation");
		ImGui::SameLine();
		if (ImGui::DragFloat3("r", &rotate[0], 0.1f)) {
			gameObject->SetRotation(rotate);
		}

		ImGui::Text("Scale   ");
		ImGui::SameLine();
		if (ImGui::DragFloat3("s", &scale[0], 0.1f))
		{
			gameObject->SetScale(scale);
		}
	}
	gameObject->ShowGizmos();
}

