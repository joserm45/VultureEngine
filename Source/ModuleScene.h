#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include"GameObject.h"

struct PhysBody3D;
struct PhysMotor3D;

class Quadtree;

class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawCubeDirectMode();
	void DrawCubeVertexArrays();
	void DrawCubeDrawElements();

	void Draw();
	void DebugDraw();

	GameObject* CreateCubePrimitive();
	GameObject* CreateSpherePrimitive(int subdivisions);
	GameObject* CreateGameObject(GameObject* gameobject);
	GameObject* GetRootGameObject() const;
	void FocusGameObject(GameObject* focused, GameObject* root);
	CompCamera* GetMainCamera() const;
	//Quadtree functions
	void GenQuadtree();
	void GetStaticObjects(GameObject* static_candidate);
	void CalculateQuadtreeSize(float3& min_point, float3& max_point);
	void CheckIfRebuildQuadtree(GameObject* go);
	bool EraseObjFromStatic(GameObject* go);

	//float my_color[] = { 0.0f, 0.0f, 1.0f };
//	GLuint texture;
	//const char ImageName[] = "Assets/lenna.png";
	bool my_tool_active = true;
	char buf[10] = "casa";
	float f = 1.0f;
	GameObject* scene_gameobject_pointer = nullptr;

	std::vector<GameObject*> GO_list;

public:

	GameObject* scene_root_gameobject = nullptr;
	GameObject* camera = nullptr;

	GameObject* parent_GO = nullptr;
	GameObject* focused_GO = nullptr;

	CompCamera* main_camera = nullptr;
	CompCamera* active_camera = nullptr;



	std::vector<GameObject*> statics_game_objects;

	Quadtree* quadtree = nullptr;
	bool rebuild_quadtree = false;

	bool draw_quadtree = false;
};
