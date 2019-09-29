#pragma once
#ifndef __PANEL_H__
#define __PANEL_H__

class ModuleWindow;

class Panel
{
public:
	ModuleWindow *WindowAccess;
	char* name = nullptr;
	bool visibility = false;
	
	Panel();
	virtual ~Panel();

	virtual void Draw() {};
	virtual void SetVisibility(bool set_value);
	virtual void ToggleVisibility();

	virtual bool IsEnabled() const;
	virtual char* GetName() const { return name; }

};

#endif

