#pragma once
#ifndef __PANEL_ABOUT_H__
#define __PANEL_ABOUT_H__

#include "Panel.h"
#include "imgui/imgui.h"

class PanelAbout : public Panel
{
public:
	PanelAbout();
	virtual ~PanelAbout();

	void Draw();

private:


};

#endif