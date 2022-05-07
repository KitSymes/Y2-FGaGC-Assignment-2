#include "Particle.h"

Particle::Particle(Geometry geometry, Material material)
{
	_gameObject = new GameObject("Particle", geometry, material);
	_rigidbody = new Rigidbody(false, Vector3(), Vector3());
	_rigidbody->SetGravity(false);
	_gameObject->AddComponent(_rigidbody);

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
	return _rigidbody;
}

bool Particle::IsActive()
{
	return _active;
}

float Particle::GetLife()
{
	return _life;
}

void Particle::SetActive(bool active)
{
	_active = active;
}

void Particle::SetLife(float life)
{
	_life = life;
}

void Particle::Update(float deltaTime, Vector3 forces)
{
	if (_active)
	{
		if (_life <= 0.0f)
			_active = false;
		else
		{
			_rigidbody->AddForce(forces);
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
