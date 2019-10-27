#ifndef __PANEL_CONSOLE_H__
#define __PANEL_CONSOLE_H__

#include "Panel.h"
#include "Imgui\imgui.h"

class PanelConsole : public Panel
{
public:

	PanelConsole();
	~PanelConsole();

	void Draw();
	void ShowLog(const char* log);
	void ShowLog(const char* log, const char* dlog);

private:

	void Clear();

	ImGuiTextBuffer buff;
	bool scroll = false;
};

#endif