#include "Panel.h"


Panel::Panel() 
{ 
	name = "panel"; 
}

Panel::~Panel() {}

bool Panel::IsActive()
{
	return active_panel;
}

void Panel::ToggleVisibility()
{
	visibility = !visibility;
}

void Panel::SetVisibility(bool set_value)
{
	visibility = set_value;
}
bool Panel::IsEnabled() const
{
	return visibility;
}
