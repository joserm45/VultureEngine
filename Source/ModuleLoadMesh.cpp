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
	

	LoadMesh("Assets/warrior.FBX");



	return ret;
}


update_status ModuleLoadMesh::Update(float dt)
{
	


	
	return UPDATE_CONTINUE;
}

update_status ModuleLoadMesh::PostUpdate(float dt)
{
	
	DrawMesh();
	
	return UPDATE_CONTINUE;
}

bool ModuleLoadMesh::CleanUp()
{
	LOG("Unloading assets");


	// detach log stream
	aiDetachAllLogStreams();

	return true;
}


void ModuleLoadMesh::LoadMesh(char* path)
{
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

		//copy normal
		fbx.num_normal = scene->mMeshes[0]->mNumFaces;
		fbx.normal = new float[fbx.num_normal * 3];
		memcpy(fbx.normal, scene->mMeshes[0]->mNormals, sizeof(float) * fbx.num_normal * 3);
		LOG("New mesh with %d normals", fbx.num_normal);

		//copy color
		fbx.num_color = scene->mMeshes[0]->mNumFaces;
		fbx.color = new float[fbx.num_color * 3];
		memcpy(fbx.color, scene->mMeshes[0]->mColors, sizeof(float) * fbx.num_color * 3);

		//copy text coordinates
		fbx.num_textcoord = scene->mNumTextures;
		fbx.textcoord = new float[fbx.num_textcoord * 2];
		memcpy(fbx.textcoord, scene->mMeshes[0]->mTextureCoords, sizeof(float) * fbx.num_textcoord * 2);

		glGenBuffers(1, (GLuint*)&(fbx.id_vertex));
		glGenBuffers(1, (GLuint*)&(fbx.id_index));

		fbx.v_size = sizeof(float) * fbx.num_vertex * 3;
		fbx.n_size = sizeof(float) * fbx.num_normal * 3;
		fbx.c_size = sizeof(float) * fbx.num_color * 3;
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

		aiReleaseImport(scene);
	}
	else
	{
		LOG("ERROR LOADING SCENE %s", path);
	}

}

void ModuleLoadMesh::DrawMesh()
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

