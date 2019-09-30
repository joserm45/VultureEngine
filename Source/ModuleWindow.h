#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void SetFullscreen(bool fullscreen);
	void SetResizable(bool resizable);
	void SetBorderless(bool borderless);
	void SetFullDesktop(bool full_desktop);
	void ChangeBrightness(float brightness);
	void ChangeWidth(int width);
	void ChangeHeight(int height);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	int width_win = SCREEN_WIDTH;
	int height_win = SCREEN_HEIGHT;
	bool fullscreen = WIN_FULLSCREEN;
	bool resizable = WIN_RESIZABLE;
	bool borderless = WIN_BORDERLESS;
	bool FullDesktop = WIN_FULLSCREEN_DESKTOP;
	float brightness = 1.0f;
};

#endif // __ModuleWindow_H__