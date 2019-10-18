#include "Globals.h"
#include "Components.h"
#include "GameObject.h"

Components::Components(GameObject* parent) : gameObject(parent)
{
}

Components::~Components()
{
}

bool Components::IsActive() const
{
	return is_active;
}

void Components::SetActive(bool active)
{
	this->is_active = active;
}

GameObject* Components::GetGameObject() const
{
	return gameObject;
}
