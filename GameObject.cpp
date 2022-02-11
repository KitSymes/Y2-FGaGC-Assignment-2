#include "GameObject.h"

GameObject::GameObject(string type, Geometry geometry, Material material) : _type(type)
{
	_parent = nullptr;
	_transform = new Transform();
	_appearance = new Appearance(geometry, material);
}

GameObject::~GameObject()
{
	if (_transform)
		delete _transform;
	_transform = nullptr;

	if (_appearance)
		delete _appearance;
	_appearance = nullptr;
}

void GameObject::Update(float t)
{
	_transform->Update(t);

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(&_transform->GetWorld(), _transform->GetWorldMatrix() * _parent->_transform->GetWorldMatrix());
	}
}

void GameObject::Draw(ID3D11DeviceContext* pImmediateContext)
{
	// NOTE: We are assuming that the constant buffers and all other draw setup has already taken place
	Geometry temp = _appearance->GetGeometryData();
	// Set vertex and index buffers
	pImmediateContext->IASetVertexBuffers(0, 1, &temp.vertexBuffer, &temp.vertexBufferStride, &temp.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(_appearance->GetGeometryData().indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pImmediateContext->DrawIndexed(_appearance->GetGeometryData().numberOfIndices, 0, 0);
}
