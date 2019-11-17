#include "CompCamera.h"
#include "Globals.h"
#include "Imgui\imgui.h"
#include "GameObject.h"
#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include"CompTransform.h"


CompCamera::CompCamera(GameObject* parent) : Components(parent)
{
	gameObject->SetName("Camera");
	frustum.type = math::PerspectiveFrustum;

	frustum.pos = math::float3::zero;
	frustum.front = math::float3::unitZ;
	frustum.up = math::float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 70.0f;

	frustum.verticalFov = DEGTORAD * fov;
	SetAspectRatio(1.3f);
}

CompCamera::~CompCamera()
{

}

void CompCamera::SetFov()
{
	frustum.verticalFov = DEGTORAD * fov;
	//frustum.horizontalFov = 2.f * atanf((tanf(frustum.verticalFov * 0.5f)) * (aspect_ratio));
	frustum.horizontalFov = atan(aspect_ratio*tan(frustum.verticalFov / 2)) * 2;
}

void CompCamera::SetVerticalFOV(float value)
{
	frustum.verticalFov = value;
	frustum.horizontalFov = 2 * atanf(tanf(value * 0.5f) * (aspect_ratio));
}
void CompCamera::SetAspectRatio(float ratio)
{
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ratio);
	projection_changed = true;
	aspect_ratio = ratio;
}

void CompCamera::DrawCamera() 
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glLineWidth(1.0f);
	glColor4f(0.1f, 0.3f, 0.7f, 0.8f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(frustum.Edge(i).a.x, frustum.Edge(i).a.y, frustum.Edge(i).a.z);
		glVertex3f(frustum.Edge(i).b.x, frustum.Edge(i).b.y, frustum.Edge(i).b.z);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnd();
	glEnable(GL_LIGHTING);
}

void CompCamera::Transform()
{
	if (this != nullptr)
	{
		math::float4x4& matrix = gameObject->GetGlobalMatrix();
		frustum.pos = matrix.TranslatePart();
		frustum.front = matrix.WorldZ().Normalized();
		frustum.up = matrix.WorldY().Normalized();
		UpdateMatrix();
	}
}

void CompCamera::Draw()
{
	if (ImGui::CollapsingHeader("Camera")) {

		ImGui::DragFloat("Near Distance", &frustum.nearPlaneDistance, 0.5, 0.0, frustum.farPlaneDistance);

		ImGui::DragFloat("Far Distance", &frustum.farPlaneDistance, 0.5);

		if (ImGui::DragFloat("Field of View", &fov, 0.5))
		{
			frustum.verticalFov = DEGTORAD * fov;
			frustum.horizontalFov = atan(aspect_ratio*tan(frustum.verticalFov / 2)) * 2;
		}
	}
	UpdateMatrix();
}

void CompCamera::UpdateMatrix()
{
	view_matrix = frustum.ViewMatrix();
	view_matrix.Transpose();

	projection_matrix = frustum.ProjectionMatrix();
	projection_matrix.Transpose();
}




float * CompCamera::GetProjectionMatrix() const
{
	return (float*)projection_matrix.v;
}

float * CompCamera::GetViewMatrix() const
{
	return (float*)view_matrix.v;
}


math::Frustum  CompCamera::GetFrustum() const
{
	return frustum;
}