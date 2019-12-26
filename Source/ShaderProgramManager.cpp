#include "ShaderProgramManager.h"
#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Application.h"

ShaderProgramManager::ShaderProgramManager()
{

}

ShaderProgramManager::~ShaderProgramManager()
{

}

bool ShaderProgramManager::Update(float dt)
{
	return true;
}

bool ShaderProgramManager::LoadShaderObject(ShaderObject* object)
{
	if (object->type == FRAGMENT_SHADER)
	{
		object->id = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else
	{
		object->id = glCreateShader(GL_VERTEX_SHADER);
	}

	glShaderSource(object->id, 1, &object->data, NULL);
	glCompileShader(object->id);
	int success;
	glGetShaderiv(object->id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		char infoLog[512];
		glGetShaderInfoLog(object->id, 512, NULL, infoLog);
		App->imgui->AddLogToConsole(("Shader compilation error : %s", infoLog));
	}

	objects.push_back(object);

	return success;
}

bool ShaderProgramManager::UnloadShaderObject(ShaderObject* object)
{
	glDeleteShader(object->id);
	return true;
}

ShaderProgram* ShaderProgramManager::CreateShaderProgram()
{
	GLuint programid = glCreateProgram();

	for (int i = 0; i < objects.size(); i++)
	{
		glAttachShader(programid, objects[i]->id);
	}

	glLinkProgram(programid);
	int success;
	glGetProgramiv(programid, GL_LINK_STATUS, &success);

	for (int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
		objects[i] = nullptr;
	}
	objects.clear();

	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(programid, 512, NULL, infoLog);
		App->imgui->AddLogToConsole(("Shader link error: %s", infoLog));
	}
	else
	{
		ShaderProgram* new_program = new ShaderProgram();
		new_program->id_shader_prog = programid;
		programs.push_back(new_program);
		objects.clear();
		return new_program;
	}
	return nullptr;
}


ShaderProgram * ShaderProgramManager::CreateWaterShaderProgram()
{
	ShaderObject* water_vertex = new ShaderObject(shader_type::VERTEX_SHADER);
	water_vertex->data = (GLchar*)water_vertex_shader;
	LoadShaderObject(water_vertex);

	ShaderObject* water_fragment = new ShaderObject(shader_type::FRAGMENT_SHADER);
	water_fragment->data = (GLchar*)water_frag_shader;
	LoadShaderObject(water_fragment);

	return CreateShaderProgram();
}

ShaderProgram* ShaderProgramManager::CreateDefaultShaderProgram()
{
	//default shader
	ShaderObject* default_vertex = new ShaderObject(shader_type::VERTEX_SHADER);
	default_vertex->data = (GLchar*)def_vertex_shader;
	LoadShaderObject(default_vertex);

	ShaderObject* default_fragment = new ShaderObject(shader_type::FRAGMENT_SHADER);
	default_fragment->data = (GLchar*)def_frag_shader;
	LoadShaderObject(default_fragment);

	return CreateShaderProgram();
}

bool ShaderProgram::UseProgram()
{
	//ONLY ONCE COMPILED
	glUseProgram(id_shader_prog);

	GLenum error = glGetError();

	if (error != GL_NO_ERROR) {
		LOG("Error binding shader! %s\n", gluErrorString(error));
	}
	return true;
}