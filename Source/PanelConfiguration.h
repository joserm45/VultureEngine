#pragma once
#ifndef __PANEL_CONFIG_H__
#define __PANEL_CONFIG_H__

#include "Panel.h"
#include "Application.h"
#include "globals.h"
#include <vector>

using namespace std;

class PanelConfiguration : public Panel
{
public:
	PanelConfiguration();
	virtual ~PanelConfiguration();

	void Draw();

private:

	vector<float> fps_log;
	vector<float> ms_log;
	const uint perfor_vec_size = 70;
	bool a = false, b = false, c = false, d = false;
};

#endif