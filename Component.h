#pragma once
#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <directxmath.h>
#include <d3d11_1.h>

class GameObject;

class Component
{
protected:
	GameObject* _gameObject;

public:
	void SetParent(GameObject* gameObject);

	GameObject* GetGameObject();

	virtual void Update(float t);
};
#endif
