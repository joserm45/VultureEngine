#include "CompCamera.h"
#include "Globals.h"
#include "Imgui\imgui.h"
#include "GameObject.h"
#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


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

void CompCamera::SetAspectRatio(float ratio)
{
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ratio);
	projection_changed = true;
	aspect_ratio = ratio;
}

void CompCamera::Draw() 
{
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
}

void CompCamera::Transform()
{
	if (this != nullptr)
	{
		math::float4x4& matrix = gameObject->GetGlobalMatrix();
		frustum.pos = matrix.TranslatePart();
		frustum.front = matrix.WorldZ().Normalized();
		frustum.up = matrix.WorldY().Normalized();
	}
}