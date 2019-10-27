#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

#include "MathGeoLib\include\Math\MathAll.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Camera";

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
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
		vec3 newPos(0, 0, 0);
		float speed = 8.0f * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed *= 2.0f;
		
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
		{
			LookAt(X);
		}

		if ((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RALT) == KEY_REPEAT) &&
			App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion(); 
			int dy = -App->input->GetMouseYMotion(); 

			if (dx != 0 || dy != 0)
			{
				float cameraRotationSpeed = speed * App->GetDt();

				LookAround(reference, (float)dy * speed, (float)dx * cameraRotationSpeed);
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

		if (App->input->GetMouseZ()) newPos -= Z * App->input->GetMouseZ() * speed;

		Position += newPos;
		Reference += newPos;

		// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

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
		}
	}
	else
		Follow();

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

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

void ModuleCamera3D::LookAt(const math::float3& reference, float radius) const
{
	math::float3 Z = -(frustum.pos - reference).Normalized(); // Direction the camera is looking at (reverse direction of what the camera is targeting)
	math::float3 X = math::Cross(math::float3(0.0f, 1.0f, 0.0f), Z).Normalized(); // X is perpendicular to vectors Y and Z
	math::float3 Y = math::Cross(Z, X); // Y is perpendicular to vectors Z and X

	(float3)frustum.front = Z;
	(float3)frustum.up = Y;

	if (radius != 0.0f)
	{
		float distance = (frustum.pos - reference).Length();
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


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
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

