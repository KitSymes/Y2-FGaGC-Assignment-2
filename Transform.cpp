#include "Transform.h"

Transform::Transform()
{
	_position = Vector3();
	XMVECTOR vectorQ = XMQuaternionRotationMatrix(XMMatrixRotationX(0.0f) * XMMatrixRotationY(0.0f) * XMMatrixRotationZ(0.0f));
	XMFLOAT4 float4Q;
	XMStoreFloat4(&float4Q, vectorQ);
	_rotation = Quaternion(float4Q.w, float4Q.x, float4Q.y, float4Q.z);
	_rotation.normalise();
	_scale = Vector3(1.0f, 1.0f, 1.0f);
}

Transform::~Transform()
{
}

void Transform::Update(float t)
{
	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	XMMATRIX rotation;
	CalculateTransformMatrixRowMajor(rotation, Vector3(), _rotation);
	XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);

	XMStoreFloat4x4(&_world, scale * rotation * translation);
}
