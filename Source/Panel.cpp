#include "Panel.h"


Panel::Panel() {}

Panel::~Panel() {}

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
