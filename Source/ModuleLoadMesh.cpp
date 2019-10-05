#include "Application.h"
#include "Globals.h"
#include "ModuleLoadMesh.h"

#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleLoadMesh::ModuleLoadMesh(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleLoadMesh::~ModuleLoadMesh()
{}


bool ModuleLoadMesh::Init()
{
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}


bool ModuleLoadMesh::Start()
{
	LOG("Loading assets");
	bool ret = true;
	
	char* path = "Assets/warrior.FBX";

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != NULL && scene->HasMeshes())
	{
		//to do

		aiReleaseImport(scene);
	}
	else
	{
		LOG("ERROR LOADING SCENE %s", path);
	}


	return ret;
}


update_status ModuleLoadMesh::Update(float dt)
{

	
	return UPDATE_CONTINUE;
}

update_status ModuleLoadMesh::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleLoadMesh::CleanUp()
{
	LOG("Unloading assets");


	// detach log stream
	aiDetachAllLogStreams();

	return true;
}


