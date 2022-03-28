#pragma once
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>
#include <vector>

#include "Vector3.h"
#include "Transform.h"
#include "Appearance.h"
#include "Component.h"

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject(string type, Geometry geometry, Material material);
	~GameObject();

	string GetType() const { return _type; }

	void SetParent(GameObject* parent) { _parent = parent; }
	GameObject* GetParent() { return _parent; }

	Transform* GetTransform() const { return _transform; }
	Appearance* GetAppearance() const { return _appearance; }

	void Update(float t);
	void Draw(ID3D11DeviceContext* pImmediateContext);

	void AddComponent(Component* component);
	template<typename T>
	T* GetComponent()
	{
		for (int i = 0; i < components.size(); i++)
			if (T* v = dynamic_cast<T*>(components[i]))
				return v;
		return nullptr;
	}

private:
	string _type;

	GameObject* _parent;

	Transform* _transform;
	Appearance* _appearance;

	vector<Component*> components;
};
#endif
