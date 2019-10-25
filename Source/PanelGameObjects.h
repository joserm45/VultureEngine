#ifndef __PANEL_GAME_OBJECTS_H__
#define __PANEL_GAME_OBJECTS_H__

#include "Panel.h"
#include "GameObject.h"

class PanelGameObjects : public Panel
{
public:
	PanelGameObjects();
	~PanelGameObjects();

	void Draw();
	void DrawPanelGameObjects(GameObject* game_object, int& node_id);

};

#endif