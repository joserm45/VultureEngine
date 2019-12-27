#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

class GameObject;

enum TYPECOMP {
	DEFAULT,
	TRANSFORM,
	MATERIAL,
	AUDIO,
	MESH,
	CAMERA,
};

class Components
{
public:

	Components();
	Components(GameObject* parent);
	virtual ~Components();

	virtual void Enable() {};
	virtual void Disable() {};

	bool IsActive() const;
	void SetActive(bool active);

	GameObject* GetGameObject() const;
	void SetGameObject(GameObject* go);
	TYPECOMP GetType() const;

protected:

	TYPECOMP type = DEFAULT;
	GameObject* gameObject;
	bool is_active = true;
	
};

#endif