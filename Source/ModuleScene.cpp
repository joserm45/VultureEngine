#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "Imgui\imgui.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "Panel.h"
#include "CompCamera.h"
#include "Quadtree.h"

#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>



ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->LookAt(vec3(0.0f, 0.0f, 0.0f));

	scene_root_gameobject = CreateGameObject(nullptr);
	scene_root_gameobject->SetName("Scene Game Objects");

	//scene_gameobject_pointer = CreateGameObject(scene_root_gameobject);
	
	//scene_gameobject_pointer->CreateComponent(MESH, 4, "BakerHouse.fbx");
	//scene_gameobject_pointer->SetName("Baker House");

	CreateCamera(true);

	/*camera = new GameObject(scene_root_gameobject);
	camera->CreateComponent(CAMERA,0,NULL);
	camera->SetName("camera");
	SetMainCamera(camera->camera);*/
	

	/*camera = CreateGameObject(GetRootGameObject());
	camera->CreateComponent(CAMERA, 0, NULL);*/
	//camera->SetPosition(math::float3(0.0, 0.0, -20.0));

	//Quadtree 
	GenQuadtree();

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");
	RELEASE(scene_root_gameobject);
	//RELEASE(camera);

	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
	if (App->input->keyboard[SDL_SCANCODE_M] == KEY_DOWN)
	{
		App->SaveProject();
	}
	if (App->input->keyboard[SDL_SCANCODE_N] == KEY_DOWN)
	{
		App->LoadProject();
	}
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);*/
	//Draw Cubes
	//DrawCubeDirectMode();
	

	//DrawCubeVertexArrays();

	//DrawCubeDrawElements();
	Draw();

	for (std::vector<GameObject*>::const_iterator i = GO_list.begin(); i != GO_list.end(); ++i)
	{
		if ((*i)->IsVisible() == true)
		{
			/*glPushMatrix();
			float4x4 matrix = (*i)->GetGlobalMatrix();
			glMultMatrixf((GLfloat*)matrix.Transposed().ptr());*/
			(*i)->Draw();
		}
			
		//(*i)->Update();
	}
	//camera->camera->DrawCamera();
	scene_root_gameobject->Update();
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	if (rebuild_quadtree) {
		rebuild_quadtree = false;

		GenQuadtree();
	}
	return UPDATE_CONTINUE;
}

GameObject * ModuleScene::CreateCamera(bool is_main_camera)
{
	camera = new GameObject(scene_root_gameobject);
	camera->CreateComponent(CAMERA,0,NULL);
	camera->SetName("Camera");

	if (is_main_camera)
		SetMainCamera(camera->camera);

	return camera;
}

void ModuleScene::DrawCubeDirectMode()
{
	//CUBE 1 DIRECT MODE
	float v0[] = { 1.0f, 1.0f, 0.0f };
	float v1[] = { 0.0f, 1.0f, 0.0f };
	float v2[] = { 0.0f, 0.0f, 0.0f };
	float v3[] = { 1.0f, 0.0f, 0.0f };
	float v4[] = { 1.0f, 0.0f, 1.0f };
	float v5[] = { 1.0f, 1.0f, 1.0f };
	float v6[] = { 0.0f, 1.0f, 1.0f };
	float v7[] = { 0.0f, 0.0f, 1.0f };

	glBegin(GL_TRIANGLES);
	//glColor3f(0, 0, 255);
	//Front
	glVertex3fv(v0);
	glVertex3fv(v1);
	glVertex3fv(v2);
	glVertex3fv(v2);
	glVertex3fv(v3);
	glVertex3fv(v0);
	//Right
	glVertex3fv(v0);
	glVertex3fv(v3);
	glVertex3fv(v4);
	glVertex3fv(v4);
	glVertex3fv(v5);
	glVertex3fv(v0);
	//Up
	glVertex3fv(v0);
	glVertex3fv(v5);
	glVertex3fv(v6);
	glVertex3fv(v6);
	glVertex3fv(v1);
	glVertex3fv(v0);
	//Back
	glVertex3fv(v7);
	glVertex3fv(v6);
	glVertex3fv(v5);
	glVertex3fv(v5);
	glVertex3fv(v4);
	glVertex3fv(v7);
	//Left
	glVertex3fv(v7);
	glVertex3fv(v2);
	glVertex3fv(v1);
	glVertex3fv(v1);
	glVertex3fv(v6);
	glVertex3fv(v7);
	//Down
	glVertex3fv(v7);
	glVertex3fv(v4);
	glVertex3fv(v3);
	glVertex3fv(v3);
	glVertex3fv(v2);
	glVertex3fv(v7);

	/*glEnable(GL_TEXTURE_2D);



	int width, height;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	//unsigned char* image = loadBMP_custom("./my_texture.bmp");;

	GLubyte checkImage[50][50][4];

	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;

			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 50, 50, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	glBindTexture(GL_TEXTURE_2D, 0);*/
	glEnd();
}

void ModuleScene::DrawCubeVertexArrays()
{
	//CUBE 2 VERTEX ARRAYS & GLDRAWARRAYS
	float v0_VA[] = { 3.0f, 1.0f, 0.0f };
	float v1_VA[] = { 2.0f, 1.0f, 0.0f };
	float v2_VA[] = { 2.0f, 0.0f, 0.0f };
	float v3_VA[] = { 3.0f, 0.0f, 0.0f };
	float v4_VA[] = { 3.0f, 0.0f, 1.0f };
	float v5_VA[] = { 3.0f, 1.0f, 1.0f };
	float v6_VA[] = { 2.0f, 1.0f, 1.0f };
	float v7_VA[] = { 2.0f, 0.0f, 1.0f };

	float vertices[] = {
		//Front
			v0_VA[0], v0_VA[1], v0_VA[2],
			v1_VA[0], v1_VA[1], v1_VA[2],
			v2_VA[0], v2_VA[1], v2_VA[2],
			v2_VA[0], v2_VA[1], v2_VA[2],
			v3_VA[0], v3_VA[1], v3_VA[2],
			v0_VA[0], v0_VA[1], v0_VA[2],
		//Right
			v0_VA[0], v0_VA[1], v0_VA[2],
			v3_VA[0], v3_VA[1], v3_VA[2],
			v4_VA[0], v4_VA[1], v4_VA[2],
			v4_VA[0], v4_VA[1], v4_VA[2],
			v5_VA[0], v5_VA[1], v5_VA[2],
			v0_VA[0], v0_VA[1], v0_VA[2],
		//Up
			v0_VA[0], v0_VA[1], v0_VA[2],
			v5_VA[0], v5_VA[1], v5_VA[2],
			v6_VA[0], v6_VA[1], v6_VA[2],
			v6_VA[0], v6_VA[1], v6_VA[2],
			v1_VA[0], v1_VA[1], v1_VA[2],
			v0_VA[0], v0_VA[1], v0_VA[2],
		//Back
			v7_VA[0], v7_VA[1], v7_VA[2],
			v6_VA[0], v6_VA[1], v6_VA[2],
			v5_VA[0], v5_VA[1], v5_VA[2],
			v5_VA[0], v5_VA[1], v5_VA[2],
			v4_VA[0], v4_VA[1], v4_VA[2],
			v7_VA[0], v7_VA[1], v7_VA[2],
		//Left
			v7_VA[0], v7_VA[1], v7_VA[2],
			v2_VA[0], v2_VA[1], v2_VA[2],
			v1_VA[0], v1_VA[1], v1_VA[2],
			v1_VA[0], v1_VA[1], v1_VA[2],
			v6_VA[0], v6_VA[1], v6_VA[2],
			v7_VA[0], v7_VA[1], v7_VA[2],
		//Down
			v7_VA[0], v7_VA[1], v7_VA[2],
			v4_VA[0], v4_VA[1], v4_VA[2],
			v3_VA[0], v3_VA[1], v3_VA[2],
			v3_VA[0], v3_VA[1], v3_VA[2],
			v2_VA[0], v2_VA[1], v2_VA[2],
			v7_VA[0], v7_VA[1], v7_VA[2] };

	uint my_id = 0;
	glGenBuffers(1, (GLuint*)&(my_id));
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor3f(255, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));

	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleScene::DrawCubeDrawElements()
{
	//CUBE 3 GLDRAWELEMENTS
	float vertices_DE[] = {
		5.0f, 1.0f, 0.0f, 
		4.0f, 1.0f, 0.0f,
		4.0f, 0.0f, 0.0f,
		5.0f, 0.0f, 0.0f,
		5.0f, 0.0f, 1.0f,
		5.0f, 1.0f, 1.0f,
		4.0f, 1.0f, 1.0f,
		4.0f, 0.0f, 1.0f
	};

	uint indices[] = {
		0, 1, 2,  2, 3, 0,
		0, 3, 4,  4, 5, 0,
		0, 5, 6,  6, 1, 0,
		7, 6, 5,  5, 4, 7,
		7, 2, 1,  1, 6, 7,
		7, 4, 3,  3, 2, 7
	};

	uint my_id2 = 0;
	uint my_indices = 0;
	glGenBuffers(1, (GLuint*)&(my_id2));
	glGenBuffers(1, (GLuint*)&(my_indices));
	glBindBuffer(GL_ARRAY_BUFFER, my_id2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeof(vertices_DE) * 3, vertices_DE, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(0, 255, 0);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleScene::Draw()
{
	//App->panel->Draw();
	list<Panel*>::const_iterator panel = App->imgui->panels.begin();
	while (panel != App->imgui->panels.end())
	{
		if ((*panel)->IsActive())
			(*panel)->Draw();
		panel++;
	}
	if (App->GetState() == ENGINE_STATE_EDITOR)
		DebugDraw();
}

void ModuleScene::DebugDraw()
{
	glDisable(GL_LIGHTING);

	if (draw_quadtree)
		quadtree->DebugDraw();

	glEnable(GL_LIGHTING);
}

GameObject* ModuleScene::CreateGameObject(GameObject* gameobject)
{
	GameObject* game_object = new GameObject(gameobject);

	return game_object;
}

GameObject* ModuleScene::GetRootGameObject() const
{
	return scene_root_gameobject;
}

void ModuleScene::FocusGameObject(GameObject* focused, GameObject* root) 
{

	if (root->GetNumChilds() > 0) {

		for (std::vector<GameObject*>::const_iterator it = root->childs.begin(); it < root->childs.end(); it++) {

			if ((*it) == focused) {
				(*it)->focused = true;
				LOG("%c focused", (*it)->GetName());
			}
			else {
				(*it)->focused = false;
				LOG("%c NOT focused", (*it)->GetName());
			}
			FocusGameObject(focused, (*it));
			focused_GO = focused;
		}
	}
}

void ModuleScene::SetMainCamera(CompCamera* cam)
{
	if (main_camera)
		main_camera->camer_active = false;

	if (cam == nullptr)
		main_camera = nullptr;
	else {
		main_camera = cam;
		main_camera->camer_active = true;
	}

}

CompCamera * ModuleScene::GetMainCamera() const
{
	return main_camera;
}

void ModuleScene::GenQuadtree()
{
	//If there's quadtree, delete it
	if (quadtree)
		RELEASE(quadtree);

	//Get a vector of static GameObjects
	statics_game_objects.clear();
	for (int i = 0; i < scene_root_gameobject->GetNumChilds(); ++i)
		GetStaticObjects(scene_root_gameobject->GetChild(i));

	//Calculate quadtree size
	float3 min_point(-30); //Minimum quadtree size
	float3 max_point(30); //Maximum quadtree size
	CalculateQuadtreeSize(min_point, max_point);

	//Create quadtree
	quadtree = new Quadtree();
	AABB root_node = AABB(min_point, max_point);
	quadtree->Boundaries(root_node);

	//Insert all the static GameObjects to the quadtree
	for (int i = 0; i < statics_game_objects.size(); ++i)
		quadtree->Insert(statics_game_objects[i]);
}

void ModuleScene::GetStaticObjects(GameObject* static_game_object)
{
	if (static_game_object->IsStatic())
		statics_game_objects.push_back(static_game_object);

	for (int i = 0; i < static_game_object->GetNumChilds(); ++i)
		GetStaticObjects(static_game_object->GetChild(i));
}

void ModuleScene::CalculateQuadtreeSize(float3& min_point, float3& max_point)
{
	for (int i = 0; i < statics_game_objects.size(); ++i) {
		//Min point
		float3 min_p = statics_game_objects[i]->BBox.minPoint;
		if (min_p.x < min_point.x)
			min_point.x = min_p.x;
		if (min_p.y < min_point.y)
			min_point.y = min_p.y;
		if (min_p.z < min_point.z)
			min_point.z = min_p.z;

		//Max point
		float3 max_p = statics_game_objects[i]->BBox.maxPoint;
		if (max_p.x > max_point.x)
			max_point.x = max_p.x;
		if (max_p.y > max_point.y)
			max_point.y = max_p.y;
		if (max_p.z > max_point.z)
			max_point.z = max_p.z;
	}
}
void ModuleScene::SetActiveAllObj(GameObject* game_object)
{
	game_object->SetVisibility(true);

	for (int i = 0; i < game_object->GetNumChilds(); ++i)
		SetActiveAllObj(game_object->GetChild(i));
}
void ModuleScene::CheckIfRebuildQuadtree(GameObject * game_object)
{
	float3 min_point = game_object->BBox.minPoint;
	float3 max_point = game_object->BBox.maxPoint;
	bool rebuild = false;

	//Min point
	if (min_point.x < quadtree->min_point.x || min_point.x > quadtree->min_point.x && min_point.x < 30
		|| min_point.y < quadtree->min_point.y || min_point.y > quadtree->min_point.y && min_point.y < 30
		|| min_point.z < quadtree->min_point.z || min_point.z > quadtree->min_point.z && min_point.y < 30)
		rebuild = true;
	//Max point
	if (max_point.x > quadtree->max_point.x || max_point.x < quadtree->max_point.x && max_point.x > 30
		|| max_point.y > quadtree->max_point.y || max_point.y < quadtree->max_point.y && max_point.y > 30
		|| max_point.z > quadtree->max_point.z || max_point.z < quadtree->max_point.z && max_point.z > 30)
		rebuild = true;

	if (rebuild)
		GenQuadtree();
}

bool ModuleScene::EraseObjFromStatic(GameObject* game_object)
{
	for (std::vector<GameObject*>::const_iterator it = statics_game_objects.begin(); it < statics_game_objects.end(); it++)
		if ((*it) == game_object) {
			statics_game_objects.erase(it);
			return true;
		}
	return false;
}


