#pragma once
#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <directxmath.h>
#include <d3d11_1.h>

#include "Vector3.h"

using namespace DirectX;

class Transform
{
public:
	Transform();
	~Transform();

	// Setters and Getters for position/rotation/scale
	void SetPosition(Vector3 position) { _position = position; }
	void SetPosition(float x, float y, float z) { _position.x = x; _position.y = y; _position.z = z; }
	Vector3 GetPosition() const { return _position; }

	void SetScale(Vector3 scale) { _scale = scale; }
	void SetScale(float x, float y, float z) { _scale.x = x; _scale.y = y; _scale.z = z; }
	Vector3 GetScale() const { return _scale; }

	void SetRotation(Vector3 rotation) { _rotation = rotation; }
	void SetRotation(float x, float y, float z) { _rotation.x = x; _rotation.y = y; _rotation.z = z; }
	Vector3 GetRotation() const { return _rotation; }

	XMFLOAT4X4 GetWorld() const { return _world; }
	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void Update(float t);
private:
	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;

	XMFLOAT4X4 _world;
};
#endif
