#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>

#include "Vector3.h"
#include "Transform.h"
#include "Appearance.h"

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject(string type, Geometry geometry, Material material);
	~GameObject();

	string GetType() const { return _type; }

	void SetParent(GameObject* parent) { _parent = parent; }

	Transform* GetTransform() const { return _transform; }
	Appearance* GetAppearance() const { return _appearance; }

	void Update(float t);
	void Draw(ID3D11DeviceContext* pImmediateContext);

private:
	string _type;

	GameObject* _parent;

	Transform* _transform;
	Appearance* _appearance;
};

