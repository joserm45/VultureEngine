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
	App->imgui->AddLogToConsole("Loading Baker House");
	bool ret = true;



	//LoadMesh("Assets/WoodenTower/woodenwatchtower2.FBX");
	//LoadMesh("Assets/Rifle/KSR-29 sniper rifle new_fbx_7.4_binary.FBX");
	LoadMesh("Assets/BakerHouse.FBX");
	//LoadMesh(NULL, true, 1);


	//LoadTexture("Assets/WoodenTower/textures/Wood_Tower_Col.jpg");
	//LoadTexture("Assets/Rifle/textures/Sniper_KSR_29_Col.jpg");
	//LoadTexture("Assets/Baker_house.png");
	//LoadTexture("Assets/Baker_house__ddsTest.DDS");
	//LoadChessTexture();
	
	

	return ret;
}


update_status ModuleImport::Update(float dt)
{




	return UPDATE_CONTINUE;
}

update_status ModuleImport::PostUpdate(float dt)
{

	for (list<mesh_data>::iterator i = mesh_list.begin(); i != mesh_list.end(); ++i)
	{
		DrawMesh(par_shape, *i);
	}


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


void ModuleImport::LoadMesh(char* path, bool is_parshape, uint i)
{

	ClearMeshData();
	if (is_parshape == false)
	{
	
		par_shape = false;
		const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
		if (scene == NULL)
			App->imgui->AddLogToConsole("Error Loading File, please try different file.");

		if (scene != NULL && scene->HasMeshes())
		{
			for (int x = 0; x < scene->mNumMeshes; x++)
			{
				aiMesh* mesh = scene->mMeshes[x];

				//copy vertices
				fbx.num_vertex = mesh->mNumVertices;
				fbx.vertex = new float[fbx.num_vertex * 3];
				memcpy(fbx.vertex, mesh->mVertices, sizeof(float) * fbx.num_vertex * 3);
				LOG("New mesh with %d vertices", fbx.num_vertex);
				
				//copy faces
				if (mesh->HasFaces())
				{
					fbx.num_index = mesh->mNumFaces * 3;
					fbx.index = new uint[fbx.num_index];
					for (uint i = 0; i < mesh->mNumFaces; ++i)
					{

						if (mesh->mFaces[i].mNumIndices != 3)
						{
							App->imgui->AddLogToConsole("WARNING, geometry face with != 3 indices!");
						}
						else
						{
							memcpy(&fbx.index[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
						}
					}
				}

				//copy normal
				if (mesh->HasNormals())
				{
					fbx.num_normal = mesh->mNumVertices;
					fbx.normal = new float[fbx.num_normal * 3];
					memcpy(fbx.normal, mesh->mNormals, sizeof(float) * fbx.num_normal * 3);
					LOG("New mesh with %d normals", fbx.num_normal);
				}

				//copy color
				/*
				if (mesh->HasVertexColors(0))
				{
				fbx.num_color = mesh->mNumVertices;
				fbx.color = new float[fbx.num_color * 4];
				for (uint i = 0; i < mesh->mNumVertices; ++i) {
				memcpy(&fbx.color[i], &mesh->mColors[0][i].r, sizeof(float));
				memcpy(&fbx.color[i + 1], &mesh->mColors[0][i].g, sizeof(float));
				memcpy(&fbx.color[i + 2], &mesh->mColors[0][i].b, sizeof(float));
				memcpy(&fbx.color[i + 3], &mesh->mColors[0][i].a, sizeof(float));
				}
				}
				*/

				//copy text coordinates
				if (mesh->HasTextureCoords(0))
				{
					fbx.num_textcoord = mesh->mNumVertices;
					fbx.textcoord = new float[fbx.num_textcoord * 2];
					//memcpy(fbx.textcoord, scene->mMeshes[0]->mTextureCoords, sizeof(float) * fbx.num_textcoord * 2);
					for (uint i = 0; i < mesh->mNumVertices; ++i)
					{
						memcpy(&fbx.textcoord[i * 2], &mesh->mTextureCoords[0][i].x, sizeof(float));
						memcpy(&fbx.textcoord[(i * 2) + 1], &mesh->mTextureCoords[0][i].y, sizeof(float));
					}

				}

				glGenBuffers(1, (GLuint*)&(fbx.id_vertex));
				glGenBuffers(1, (GLuint*)&(fbx.id_index));
				glGenBuffers(1, (GLuint*)&(fbx.id_texture));

				glBindBuffer(GL_ARRAY_BUFFER, fbx.id_vertex);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * fbx.num_vertex * 3, fbx.vertex, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbx.id_index);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * fbx.num_index, fbx.index, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				glBindBuffer(GL_ARRAY_BUFFER, fbx.id_texture);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * fbx.num_textcoord * 2, fbx.textcoord, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				//create gameobject
				mesh_list.push_back(fbx);
				GameObject* game_object = App->scene_intro->CreateGameObject(App->scene_intro->GetRootGameObject());
				game_object->CreateComponent(MESH,0, path);
				//game_object->CreateComponent(MATERIAL, 0, path);
				std::string path_name = path;
				std::string namedotfbx = path_name.substr(path_name.find_last_of("/\\") + 1);
				std::string::size_type const p(namedotfbx.find_last_of('.'));
				std::string name_fbx = namedotfbx.substr(0, p);;
				char* go_name = new char[name_fbx.size() + 1];
				name_fbx.copy(go_name, name_fbx.size() + 1);
				go_name[name_fbx.size()] = '\0';
				game_object->SetName(go_name);

				//Material
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				if (material != nullptr) {
					uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
					aiString path_material;
					material->GetTexture(aiTextureType_DIFFUSE, 0, &path_material);
					if(path_material.length != 0)
						game_object->CreateComponent(MATERIAL, 0, path_material.data);
				}
			}
		}
		else
		{
			LOG("ERROR LOADING SCENE %s", path);
		}
		aiReleaseImport(scene);
	}
	else if (is_parshape == true)
	{
		//ClearMeshData();
		par_shape = true;
		if (i == 1)
			shape = par_shapes_create_cube();
		else if (i == 2)
			shape = par_shapes_create_subdivided_sphere(5);
		/*
		else if (i == 3)
		shape = par_shapes_create_icosahedron();
		else if (i == 4)
		shape = par_shapes_create_dodecahedron();
		else if (i == 5)
		shape = par_shapes_create_octahedron();
		else if (i == 6)
		shape = par_shapes_create_tetrahedron();
		*/
		//copy vertices
		fbx.num_vertex = shape->npoints;
		fbx.vertex = new float[fbx.num_vertex * 3];
		memcpy(fbx.vertex, shape->points, sizeof(float) * fbx.num_vertex * 3);
		LOG("New mesh with %d vertices", fbx.num_vertex);

		//copy faces
		fbx.num_index = shape->ntriangles * 3;
		fbx.index = new uint[fbx.num_index];
		memcpy(fbx.index, shape->triangles, fbx.num_index * sizeof(PAR_SHAPES_T));

		//copy normals
		fbx.num_normal = shape->npoints;
		fbx.normal = new float[fbx.num_normal * 3];
		memcpy(fbx.normal, shape->points, sizeof(float) * fbx.num_normal * 3);
		LOG("New mesh with %d normals", fbx.num_normal);

		glGenBuffers(1, (GLuint*)&(fbx.id_vertex));
		glGenBuffers(1, (GLuint*)&(fbx.id_index));

		glBindBuffer(GL_ARRAY_BUFFER, fbx.id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * fbx.num_vertex * 3, fbx.vertex, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbx.id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * fbx.num_index, fbx.index, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh_list.push_back(fbx);
		GameObject* game_object = App->scene_intro->CreateGameObject(App->scene_intro->GetRootGameObject());
		game_object->CreateComponent(MESH, 0);
		game_object->CreateComponent(MATERIAL, 0);
		if (i == 1)
			game_object->SetName("Cube");
		else if (i == 2)
			game_object->SetName("Sphere");

		par_shapes_free_mesh(shape);
	}

}

void ModuleImport::LoadTexture(const char* path)
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	ilGenImages(1, &texture.texture);
	ilBindImage(texture.texture);
	if (ilLoad(IL_TYPE_UNKNOWN, path) == IL_TRUE)
	{
		texture.texture = ilutGLBindTexImage();
		texture.height = ilGetInteger(IL_IMAGE_WIDTH);
		texture.widht = ilGetInteger(IL_IMAGE_HEIGHT);
		/*
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		*/
		glBindTexture(GL_TEXTURE_2D, 0);
		App->imgui->AddLogToConsole("Texture Loaded Succesfuly");
	}
	else
		App->imgui->AddLogToConsole("Texture Not Loaded");
	
	ilDeleteImages(1, &texture.texture);
}

void ModuleImport::DrawMesh(bool is_parshape, mesh_data fbx)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture.texture);
	glBindBuffer(GL_ARRAY_BUFFER, fbx.id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbx.id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, fbx.id_texture);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	if (is_parshape == false)
	{
		glDrawElements(GL_TRIANGLES, fbx.num_index, GL_UNSIGNED_INT, NULL);
	}
	else if (is_parshape == true)
	{
		glDrawElements(GL_TRIANGLES, fbx.num_index, GL_UNSIGNED_SHORT, NULL);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, NULL);
}


void ModuleImport::ClearMeshData()
{
	for (list<mesh_data>::iterator i = mesh_list.begin(); i != mesh_list.end(); ++i)
	{
		if (i->index != nullptr)
		{
			delete[] i->index;
			i->index = nullptr;
		}
		if (i->vertex != nullptr)
		{
			delete[] i->vertex;
			i->vertex = nullptr;
		}
		if (i->normal != nullptr)
		{
			delete[] i->normal;
			i->normal = nullptr;
		}
		if (i->color != nullptr)
		{
			delete[] i->color;
			i->color = nullptr;
		}
		//if (par_shape == false)
		{
			if (i->textcoord != nullptr)
			{
				//delete[] i->textcoord;
				i->textcoord = nullptr;
			}
		}
	}
	mesh_list.clear();

}

void ModuleImport::LoadChessTexture()
{
	GLubyte chessImage[50][50][4];

	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;

			chessImage[i][j][0] = (GLubyte)c;
			chessImage[i][j][1] = (GLubyte)c;
			chessImage[i][j][2] = (GLubyte)c;
			chessImage[i][j][3] = (GLubyte)255;
		}
	}

	glGenTextures(1, (GLuint*)&(texture.texture));
	glBindTexture(GL_TEXTURE_2D, texture.texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 50, 50, 0, GL_RGBA, GL_UNSIGNED_BYTE, chessImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/*
void ModuleImport::LoadParShape(uint i)
{
	ClearMeshData(shape_data);

	if(i == 1)
		shape = par_shapes_create_cube();
	else if(i == 2)
		shape = par_shapes_create_subdivided_sphere(5);
	/*
	else if (i == 3)
		shape = par_shapes_create_icosahedron();
	else if (i == 4)
		shape = par_shapes_create_dodecahedron();
	else if (i == 5)
		shape = par_shapes_create_octahedron();
	else if (i == 6)
		shape = par_shapes_create_tetrahedron();
	*//*
	//copy vertices
	shape_data.num_vertex = shape->npoints;
	shape_data.vertex = new float[shape_data.num_vertex * 3];
	memcpy(shape_data.vertex, shape->points, sizeof(float) * shape_data.num_vertex * 3);
	LOG("New mesh with %d vertices", shape_data.num_vertex);

	//copy faces
	shape_data.num_index = shape->ntriangles * 3;
	shape_data.index = new uint[shape_data.num_index];
	memcpy(shape_data.index, shape->triangles, shape_data.num_index * sizeof(PAR_SHAPES_T));

	//copy normals
	shape_data.num_normal = shape->npoints;
	shape_data.normal = new float[shape_data.num_normal * 3];
	memcpy(shape_data.normal, shape->points, sizeof(float) * shape_data.num_normal * 3);
	LOG("New mesh with %d normals", shape_data.num_normal);

	glGenBuffers(1, (GLuint*)&(shape_data.id_vertex));
	glGenBuffers(1, (GLuint*)&(shape_data.id_index));

	glBindBuffer(GL_ARRAY_BUFFER, shape_data.id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * shape_data.num_vertex * 3, shape_data.vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape_data.id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * shape_data.num_index, shape_data.index, GL_STATIC_DRAW);

	par_shapes_free_mesh(shape);
}
*/

/*
void ModuleImport::DrawParShape()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	//glColor3f(0, 255, 0);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, shape_data.num_index * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}
*/