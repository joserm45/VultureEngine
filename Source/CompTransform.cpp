#include "Globals.h"
#include "CompTransform.h"
#include "GameObject.h"
#include "imgui.h"

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

void CompTransform::Draw()
{
	if (ImGui::CollapsingHeader("Transform")) {

		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::DragFloat3("p", &position[0], 0.1f);

		ImGui::Text("Rotation");
		ImGui::SameLine();
		if (ImGui::DragFloat3("r", &rotation[0], 0.1f)) {
			SetRotation(rotation);
		}

		ImGui::Text("Scale   ");
		ImGui::SameLine();
		ImGui::DragFloat3("s", &scale[0], 0.1f);


	}

}

