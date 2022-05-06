#include "Particle.h"

Particle::Particle(Geometry geometry, Material material, bool gravity)
{
	_gameObject = new GameObject("Particle", geometry, material);
	_gameObject->GetTransform()->SetScale(0.1f, 0.1f, 0.1f);
	_rigidBody = new Rigidbody(false, Vector3(), Vector3());
	_rigidBody->SetGravity(gravity);
	_gameObject->AddComponent(_rigidBody);

	_active = false;
	_life = 0.0f;
}

Particle::~Particle()
{
	delete _gameObject;
}

GameObject* Particle::GetGameObject()
{
	return _gameObject;
}

Rigidbody* Particle::GetRigidbody()
{
	return _rigidBody;
}

bool Particle::IsActive()
{
	return _active;
}

void Particle::SetActive(bool active)
{
	_active = active;
}

void Particle::Update(float deltaTime)
{
	if (_active)
	{
		if (_life <= 0.0f)
			_active = false;
		else
		{
			_gameObject->Update(deltaTime);
			_life -= deltaTime;
		}
	}
}

void Particle::Draw(ID3D11DeviceContext* pImmediateContext)
{
	if (_active)
		_gameObject->Draw(pImmediateContext);
}
