#ifndef __PANEL_H__
#define __PANEL_H__


class Panel
{
public:

	char* name = nullptr;
	bool visibility = false;
	
	Panel();
	virtual ~Panel();

	virtual void Draw() {};
	virtual void SetVisibility(bool set_value);
	virtual void ToggleVisibility();

	virtual bool IsEnabled() const;
	virtual char* GetName() const { return name; }

protected:

	bool resize = true;
	bool active_panel = false;
	int x, y, w, h = 0;
};

#endif

