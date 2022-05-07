#include "RainParticleSystem.h"

RainParticleSystem::RainParticleSystem(Geometry geometry, Material material, Vector3 spawnMin, Vector3 spawnMax) : ParticleSystem(geometry, material)
{
	for (int i = 0; i < 100; i++)
	{
		_particlePool.push_back(CreateParticle());
	}
	_timeUntilNextSpawn = 0.0f;

	_spawnMin = spawnMin;
	_spawnMax = spawnMax;
	_windDirection = Vector3();
	_gravity = Vector3(0.0f, -9.8f, 0.0f);
}

void RainParticleSystem::SetWindDirection(Vector3 direction)
{
	_windDirection = direction;
}

void RainParticleSystem::Update(float deltaTime)
{
	if (_enabled)
	{
		if (_timeUntilNextSpawn <= 0.0f)
		{
			_timeUntilNextSpawn = 0.01f;
			Particle* particle = GetFirstInactive();

			if (particle == nullptr)
			{
				particle = CreateParticle();
				_particlePool.push_back(particle);
				Debug::GetInstance().WriteLine("Generated new Rain Particle");
			}

			particle->GetRigidbody()->Reset();
			Vector3 offset = _spawnMax - _spawnMin;
			offset.x *= ((rand() % 1000) / 1000.0f);
			offset.y *= ((rand() % 1000) / 1000.0f);
			offset.z *= ((rand() % 1000) / 1000.0f);
			particle->GetGameObject()->GetTransform()->SetPosition(Vector3(_spawnMin + offset));
			particle->SetLife(5.0f);
			particle->SetActive(true);
		}
		_timeUntilNextSpawn -= deltaTime;
	}

	_forces += _windDirection;
	_forces += _gravity;

	Vector3 forcesNormal = _forces.Normalised();
	Vector3 down = Vector3(0.0f, -1.0f, 0.0f);
	float dot = Vector3(0.0f, 1.0f, 0.0f).DotProduct(forcesNormal);

	Quaternion q;
	if (dot < -0.999999f || dot > 0.999999f)
	{

	}
	else
	{
		Vector3 a = down.CrossProduct(forcesNormal);
		q.i = a.x;
		q.j = a.y;
		q.k = a.z;
		q.r = sqrt(pow(down.Magnitude(), 2) * pow(forcesNormal.Magnitude(), 2)) + dot;
	}

	for (Particle* particle : _particlePool)
	{
		particle->GetGameObject()->GetTransform()->SetRotation(q);
		particle->Update(deltaTime, _forces);
	}

	ParticleSystem::Update(deltaTime);
}

Particle* RainParticleSystem::CreateParticle()
{
	Particle* particle = ParticleSystem::CreateParticle();
	particle->GetGameObject()->GetTransform()->SetScale(0.05f, 0.1f, 0.05f);
	//particle->GetRigidbody()->SetGravity(true);
	particle->GetRigidbody()->SetGravityLimit(-1.0f);
	return particle;
}
