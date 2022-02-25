#pragma once
#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <directxmath.h>
#include <d3d11_1.h>

#include "Vector3.h"
#include "Quaternion.h"

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

	void SetRotation(Vector3 rotation) { SetRotation(rotation.x, rotation.y, rotation.z); }
	void SetRotation(float x, float y, float z)
	{
		XMVECTOR vectorQ = XMQuaternionRotationMatrix(XMMatrixRotationX(x) * XMMatrixRotationY(y) * XMMatrixRotationZ(z));
		SetRotation(vectorQ);
	}
	void SetRotationD(Vector3 rotation) { SetRotationD(rotation.x, rotation.y, rotation.z); }
	void SetRotationD(float x, float y, float z)
	{
		XMVECTOR vectorQ = XMQuaternionRotationMatrix(
			XMMatrixRotationX(XMConvertToRadians(x)) *
			XMMatrixRotationY(XMConvertToRadians(y)) *
			XMMatrixRotationZ(XMConvertToRadians(z)));
		SetRotation(vectorQ);
	}
	void SetRotation(XMVECTOR vector)
	{
		XMFLOAT4 float4Q;
		XMStoreFloat4(&float4Q, vector);
		_rotation.r = float4Q.w;
		_rotation.i = float4Q.x;
		_rotation.j = float4Q.y;
		_rotation.k = float4Q.z;
		_rotation.normalise();
	}
	Quaternion GetRotation() const { return _rotation; }

	XMFLOAT4X4 GetWorld() const { return _world; }
	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void Update(float t);
private:
	Vector3 _position;
	Quaternion _rotation;
	Vector3 _scale;

	XMFLOAT4X4 _world;
};
#endif
