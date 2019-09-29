#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "glew\include\GL\glew.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Window";
	
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	if (fullscreen == true)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}
	else if (fullscreen == false)
	{
		SDL_SetWindowFullscreen(window, 0);
		
	}
}

void ModuleWindow::SetResizable(bool resizable)
{
	if (resizable == true)
	{
		SDL_WINDOW_RESIZABLE;
	}
	else if(resizable == false)
	{
		SDL_WINDOWEVENT_RESIZED;
	}
}

void ModuleWindow::SetBorderless(bool borderless)
{
	if (borderless == true)
	{
		SDL_SetWindowBordered(window, SDL_FALSE);
	}
	else if (borderless == false)
	{
		SDL_SetWindowBordered(window, SDL_TRUE);
	}
}

void ModuleWindow::SetFullDesktop(bool FullDesktop)
{
	if (FullDesktop == true)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else if (FullDesktop == false)
	{
		SDL_SetWindowFullscreen(window, 0);
	}
}

void ModuleWindow::ChangeBrightness(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
}

void ModuleWindow::ChangeWidth(int width)
{
	width_win = width;
	SDL_SetWindowSize(window, width, height_win);
	App->renderer3D->OnResize(width, height_win);
}

void ModuleWindow::ChangeHeight(int height)
{
	height_win = height;
	SDL_SetWindowSize(window, width_win, height);
	App->renderer3D->OnResize(width_win, height);
}
