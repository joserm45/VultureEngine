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
		//copy vertices
		fbx.num_vertex = scene->mMeshes[0]->mNumVertices;
		fbx.vertex = new float[fbx.num_vertex * 3];
		memcpy(fbx.vertex, scene->mMeshes[0]->mVertices, sizeof(float) * fbx.num_vertex * 3);
		LOG("New mesh with %d vertices", fbx.num_vertex);

		//copy faces
		if (scene->mMeshes[0]->HasFaces())
		{
			fbx.num_index = scene->mMeshes[0]->mNumFaces * 3;
			fbx.index = new uint[fbx.num_index];
			for (uint i = 0; i < scene->mMeshes[0]->mNumFaces; ++i)
			{
				
				if (scene->mMeshes[0]->mFaces[i].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&fbx.index[i * 3], scene->mMeshes[0]->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}
		}
		
		glGenBuffers(1, (GLuint*)&(fbx.id_vertex));
		glGenBuffers(1, (GLuint*)&(fbx.id_index));
		glBindBuffer(GL_ARRAY_BUFFER, fbx.id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * fbx.num_vertex * 3, fbx.vertex, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbx.id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * fbx.num_index, fbx.index, GL_STATIC_DRAW);

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
	
	glEnableClientState(GL_VERTEX_ARRAY);
	//glColor3f(0, 255, 0);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, fbx.num_index * 3, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	return UPDATE_CONTINUE;
}

bool ModuleLoadMesh::CleanUp()
{
	LOG("Unloading assets");


	// detach log stream
	aiDetachAllLogStreams();

	return true;
}


