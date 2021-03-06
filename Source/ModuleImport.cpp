#include "Application.h"
#include "Globals.h"
#include "ModuleImport.h"
#include "CompMesh.h"
#include "CompTransform.h"

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

#include "ShaderProgramManager.h"



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
	//App->imgui->AddLogToConsole("Loading Baker House");
	App->imgui->AddLogToConsole("Loading Street Scene");
	bool ret = true;



	//LoadMesh("Assets/WoodenTower/woodenwatchtower2.FBX");
	//LoadMesh("Assets/Rifle/KSR-29 sniper rifle new_fbx_7.4_binary.FBX");
	//LoadMesh("Assets/BakerHouse.FBX");

	LoadMesh("Assets/scene.DAE");
	GameObject *street = App->scene_intro->scene_root_gameobject->childs[1];
	street->SetName("Street Scene");

	//street->SetRotation(float3(-45, 0, 0));
	//street->childs[0]->SetPosition(float3(0, 0, 45));
	//street->childs[0]->CreateComponent(MATERIAL, 0, "Assets/textures/Building_V01_C.PNG");

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

	//for (list<mesh_data>::iterator i = mesh_list.begin(); i != mesh_list.end(); ++i)
	//{
		//DrawMesh(fbx.par_shape, *i);
	//}


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
	
		fbx.par_shape = false;
		const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
		App->imgui->AddLogToConsole("Initialazing Assimp Correctly");
		if (scene == NULL)
			App->imgui->AddLogToConsole("Error Loading File, please try different file.");

		App->imgui->AddLogToConsole("Opening file: ", path);

		if (scene != NULL && scene->HasMeshes())
		{
			App->scene_intro->parent_GO = App->scene_intro->CreateGameObject(App->scene_intro->GetRootGameObject());
			aiNode* rootNode = scene->mRootNode;
			for (int i = 0; i < rootNode->mNumChildren; ++i)
			{
				LoadChilds(scene, rootNode->mChildren[i], App->scene_intro->parent_GO, path);
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
		fbx.par_shape = true;
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
		string str = "New mesh with " + std::to_string(fbx.num_vertex) + " vertices";
		App->imgui->AddLogToConsole(str.c_str());

		//copy faces
		fbx.num_index = shape->ntriangles * 3;
		fbx.index = new uint[fbx.num_index];
		memcpy(fbx.index, shape->triangles, fbx.num_index * sizeof(PAR_SHAPES_T));

		//copy normals
		fbx.num_normal = shape->npoints;
		fbx.normal = new float[fbx.num_normal * 3];
		memcpy(fbx.normal, shape->points, sizeof(float) * fbx.num_normal * 3);
		str = "New mesh with " + std::to_string(fbx.num_vertex) + " normals";
		App->imgui->AddLogToConsole(str.c_str());

		glGenBuffers(1, (GLuint*)&(fbx.id_vertex));
		glGenBuffers(1, (GLuint*)&(fbx.id_index));

		glBindBuffer(GL_ARRAY_BUFFER, fbx.id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * fbx.num_vertex * 3, fbx.vertex, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbx.id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * fbx.num_index, fbx.index, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh_list.push_back(fbx);
		last_GO = App->scene_intro->CreateGameObject(App->scene_intro->GetRootGameObject());
		last_GO->CreateComponent(MESH, 0);
		last_GO->CreateComponent(MATERIAL, 0);
		if (i == 1)
			last_GO->SetName("Cube");
		else if (i == 2)
			last_GO->SetName("Sphere");

		App->scene_intro->GO_list.push_back(last_GO);
		
		par_shapes_free_mesh(shape);
	}
	
}

void ModuleImport::LoadChilds(const aiScene* scene, aiNode* node, GameObject* game_object, const char* path)
{
	if (node->mNumMeshes > 0)
	{

		aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];

		//copy vertices
		fbx.num_vertex = mesh->mNumVertices;
		fbx.vertex = new float[fbx.num_vertex * 3];
		memcpy(fbx.vertex, mesh->mVertices, sizeof(float) * fbx.num_vertex * 3);
		string str = "New mesh with " + std::to_string(fbx.num_vertex) + " vertices";
		App->imgui->AddLogToConsole(str.c_str());

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
			string str = "New mesh with " + std::to_string(fbx.num_vertex) + " normals";
			App->imgui->AddLogToConsole(str.c_str());
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
		//mesh_list.push_back(fbx);

		
		//game_object->CreateComponent(MATERIAL, 0, path);

		std::string path_name = node->mName.C_Str();
		std::string namedotfbx = path_name.substr(path_name.find_last_of("/\\") + 1);
		std::string::size_type const p(namedotfbx.find_last_of('.'));
		std::string name_fbx = namedotfbx.substr(0, p);;

		char* go_name = new char[name_fbx.size() + 1];

		name_fbx.copy(go_name, name_fbx.size() + 1);
		go_name[name_fbx.size()] = '\0';
		game_object->SetName(go_name);



		//Material
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString path_material;
		if (material != nullptr) {
			uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path_material);
		}

		//Transformations
		aiVector3D position, scale;
		aiQuaternion rotation;
		node->mTransformation.Decompose(scale, rotation, position);
		float3 pos(position.x, position.y, position.z);
		//float3 pos(position.x / 2, position.y / 2, position.z / 2);
		float3 scale_f(scale.x, scale.y, scale.z);
		//Quat rotation_q(rotation.x/2, rotation.y/2, rotation.z/2, rotation.w/2);
		Quat rotation_q(rotation.x, rotation.y, rotation.z, rotation.w);
		float3 rot = rotation_q.ToEulerXYZ(); 
		rot *= RADTODEG;


		if (scene->mNumMeshes != 0)
		{
			last_GO = App->scene_intro->CreateGameObject(game_object);
			if(path == "Assets/scene.DAE")
			{
				//last_GO->SetPosition(pos);
				last_GO->SetPosition(pos/2);
				//last_GO->SetRotation(rot);
				last_GO->SetScale(scale_f);
			}
			last_GO->CreateComponent(MESH, 0, path);
			if(material != nullptr)
				last_GO->CreateComponent(MATERIAL, 0, path_material.data);
			last_GO->mesh->SetTexture(texture);
			last_GO->SetName(go_name);
		}

		App->scene_intro->GO_list.push_back(last_GO);
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		LoadChilds(scene, node->mChildren[i], game_object, path);
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

	App->imgui->AddLogToConsole("Loading Texture");

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
		App->imgui->AddLogToConsole("Texture Loaded Succesfuly: ", path);
		string size_text = "Texture size: " + std::to_string(texture.widht) + " X " + std::to_string(texture.height);
		App->imgui->AddLogToConsole(size_text.c_str());
		


		//DROPPING TEXTURE FILE TO DO: 
		//FIND SELECTED GAMEOBJECT AND APPLY "->mesh->SetTexture(texture);" TO IT!!!!!!!
		//SEE BELOW
		//if(last_GO->focused)

		
		//last_GO->mesh->SetTexture(texture);

	}
	else
		App->imgui->AddLogToConsole("Texture Not Loaded");
	
	
	ilDeleteImages(1, &texture.texture);
}

/*
void ModuleImport::DrawMesh(bool is_parshape, mesh_data fbx)
{
	glDisable(GL_LIGHTING);
	//testing transformation
	glPushMatrix();
	glMultMatrixf(last_GO->GetGlobalMatrix().Transposed().ptr());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture.texture);
	glBindBuffer(GL_ARRAY_BUFFER, fbx.id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbx.id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, fbx.id_texture);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	if (fbx.par_shape == false)
	{
		glDrawElements(GL_TRIANGLES, fbx.num_index, GL_UNSIGNED_INT, NULL);
	}
	else if (fbx.par_shape == true)
	{
		glDrawElements(GL_TRIANGLES, fbx.num_index, GL_UNSIGNED_SHORT, NULL);
	}
	//glDisable(GL_LIGHTING);
	last_GO->DrawBBox();
	//glEnable(GL_LIGHTING);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, NULL);

	glPopMatrix();
}
*/

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

void ModuleImport::LoadChessTexture(GameObject* selected)
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


	//DROPPING TEXTURE FILE TO DO: 
	//FIND SELECTED GAMEOBJECT AND APPLY "->mesh->SetTexture(texture);" TO IT!!!!!!!
	//SEE BELOW
	//if(last_GO->focused)
	selected->mesh->SetTexture(texture);


	glBindTexture(GL_TEXTURE_2D, 0);
}

uint ModuleImport::LoadTextureData(const char* name)
{
	std::string path = "Library/Shaders/";
	path += name;
	path += ".dds";


	ILuint imageID;				// Create an image ID as a ULuint

	GLuint textureID;			// Create a texture ID as a GLuint

	ILboolean success;			

	ILenum error;				

	ilGenImages(1, &imageID); 		// Generate the image ID

	ilBindImage(imageID); 			// Bind image

	success = ilLoadImage(path.c_str()); 	// Load the image file

									
	if (success)
	{

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		last_tex_width = ImageInfo.Width;
		last_tex_height = ImageInfo.Height;

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		if (!success)
		{
			error = ilGetError();
			LOG("Image conversion failed - IL reports error: %s ", iluErrorString(error));
			exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureID);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		switch (wrap)
		{
		case REPEAT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		case MIRRORED_REPEAT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			break;
		case CLAMP_TO_EDGE:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		case CLAMP_TO_BORDER:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			break;
		default:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D,	0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH),	ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());			

	}
	else // If we failed to open the image
	{
		error = ilGetError();
		LOG("Image load failed - IL reports error: %s", iluErrorString(error));
		return 0;
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	LOG("Texture creation successful.");

	return textureID; // Return the GLuint to the texture so you can use it!
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