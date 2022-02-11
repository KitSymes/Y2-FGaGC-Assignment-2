#include "Transform.h"

Transform::Transform()
{
	_position = Vector3();
	_rotation = Vector3();
	_scale = Vector3(1.0f, 1.0f, 1.0f);
}

Transform::~Transform()
{
}

void Transform::Update(float t)
{
	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	XMMATRIX rotation = XMMatrixRotationX(_rotation.x) * XMMatrixRotationY(_rotation.y) * XMMatrixRotationZ(_rotation.z);
	XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);

	XMStoreFloat4x4(&_world, scale * rotation * translation);
}
