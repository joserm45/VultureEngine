#include "Application.h"
#include "Globals.h"
#include "ModuleImport.h"

#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"
#include "DevIL\include\ilut.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#pragma comment (lib, "DevIL\\libx86\\DevIL.lib")
#pragma comment (lib, "DevIL\\libx86\\ILU.lib")
#pragma comment (lib, "DevIL\\libx86\\ILUT.lib")




ModuleImport::ModuleImport(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImport::~ModuleImport()
{}


bool ModuleImport::Init()
{
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}


bool ModuleImport::Start()
{
	LOG("Loading assets");
	bool ret = true;
	

	LoadMesh("Assets/warrior.FBX");



	return ret;
}


update_status ModuleImport::Update(float dt)
{
	


	
	return UPDATE_CONTINUE;
}

update_status ModuleImport::PostUpdate(float dt)
{
	
	DrawMesh();
	
	return UPDATE_CONTINUE;
}

bool ModuleImport::CleanUp()
{
	LOG("Unloading assets");

	ClearMeshData();
	

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}


void ModuleImport::LoadMesh(char* path)
{
	ClearMeshData();

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != NULL && scene->HasMeshes())
	{
		for (int x = 0; x < scene->mNumMeshes; x++)
		{
			//copy vertices
			fbx.num_vertex = scene->mMeshes[x]->mNumVertices;
			fbx.vertex = new float[fbx.num_vertex * 3];
			memcpy(fbx.vertex, scene->mMeshes[x]->mVertices, sizeof(float) * fbx.num_vertex * 3);
			LOG("New mesh with %d vertices", fbx.num_vertex);

			//copy faces
			if (scene->mMeshes[x]->HasFaces())
			{
				fbx.num_index = scene->mMeshes[x]->mNumFaces * 3;
				fbx.index = new uint[fbx.num_index];
				for (uint i = 0; i < scene->mMeshes[x]->mNumFaces; ++i)
				{

					if (scene->mMeshes[x]->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&fbx.index[i * 3], scene->mMeshes[x]->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}

			//copy normal
			if (scene->mMeshes[x]->HasNormals())
			{
				fbx.num_normal = scene->mMeshes[x]->mNumVertices;
				fbx.normal = new float[fbx.num_normal * 3];
				memcpy(fbx.normal, scene->mMeshes[x]->mNormals, sizeof(float) * fbx.num_normal * 3);
				LOG("New mesh with %d normals", fbx.num_normal);
			}

			//copy color
			if (scene->mMeshes[x]->HasVertexColors(0))
			{
				fbx.num_color = scene->mMeshes[x]->mNumVertices;
				fbx.color = new float[fbx.num_color * 4];
				for (uint i = 0; i < scene->mMeshes[x]->mNumVertices; ++i) {
					memcpy(&fbx.color[i], &scene->mMeshes[x]->mColors[0][i].r, sizeof(float));
					memcpy(&fbx.color[i + 1], &scene->mMeshes[x]->mColors[0][i].g, sizeof(float));
					memcpy(&fbx.color[i + 2], &scene->mMeshes[x]->mColors[0][i].b, sizeof(float));
					memcpy(&fbx.color[i + 3], &scene->mMeshes[x]->mColors[0][i].a, sizeof(float));
				}
			}

			//copy text coordinates
			if (scene->mMeshes[x]->HasTextureCoords(0))
			{
				fbx.num_textcoord = scene->mMeshes[x]->mNumVertices;
				fbx.textcoord = new float[fbx.num_textcoord * 2];
				//memcpy(fbx.textcoord, scene->mMeshes[0]->mTextureCoords, sizeof(float) * fbx.num_textcoord * 2);
				for (uint i = 0; i < scene->mMeshes[x]->mNumVertices; ++i)
				{
					memcpy(&fbx.textcoord[i], &scene->mMeshes[x]->mTextureCoords[0][i].x, sizeof(float));
					memcpy(&fbx.textcoord[i], &scene->mMeshes[x]->mTextureCoords[0][i].y, sizeof(float));
				}
			}

			glGenBuffers(1, (GLuint*)&(fbx.id_vertex));
			glGenBuffers(1, (GLuint*)&(fbx.id_index));

			fbx.v_size = sizeof(float) * fbx.num_vertex * 3;
			fbx.n_size = sizeof(float) * fbx.num_normal * 3;
			fbx.c_size = sizeof(float) * fbx.num_color * 4;
			fbx.t_size = sizeof(float) * fbx.num_textcoord * 2;

			glBindBuffer(GL_ARRAY_BUFFER, fbx.id_vertex);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * fbx.num_vertex * 3, fbx.vertex, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, fbx.v_size + fbx.n_size + fbx.c_size + fbx.t_size, 0, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, fbx.v_size, fbx.vertex);
			glBufferSubData(GL_ARRAY_BUFFER, fbx.v_size, fbx.n_size, fbx.normal);
			glBufferSubData(GL_ARRAY_BUFFER, fbx.v_size + fbx.n_size, fbx.c_size, fbx.color);
			glBufferSubData(GL_ARRAY_BUFFER, fbx.v_size + fbx.n_size + fbx.c_size, fbx.t_size, fbx.textcoord);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbx.id_index);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * fbx.num_index, fbx.index, GL_STATIC_DRAW);


			//create gameobject



		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("ERROR LOADING SCENE %s", path);
	}

}

void ModuleImport::DrawMesh()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glColor3f(0, 255, 0);

	GLsizeiptr n_offset = fbx.v_size;
	GLsizeiptr c_offset = n_offset + fbx.n_size;
	GLsizeiptr t_offset = c_offset + fbx.c_size;

	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glNormalPointer(GL_FLOAT, 0, (void*)n_offset);
	glColorPointer(3, GL_FLOAT, 0, (void*)c_offset);
	glTexCoordPointer(2, GL_FLOAT, 0, (void*)t_offset);

	glDrawElements(GL_TRIANGLES, fbx.num_index * 3, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void ModuleImport::ClearMeshData()
{
	if (fbx.index != nullptr)
	{
		delete[] fbx.index;
		fbx.index = nullptr;
	}
	if (fbx.vertex != nullptr)
	{
		delete[] fbx.vertex;
		fbx.vertex = nullptr;
	}
	if (fbx.normal != nullptr)
	{
		delete[] fbx.normal;
		fbx.normal = nullptr;
	}
	if (fbx.color != nullptr)
	{
		delete[] fbx.color;
		fbx.color = nullptr;
	}
	if (fbx.textcoord != nullptr)
	{
		delete[] fbx.textcoord;
		fbx.textcoord = nullptr;
	}
}