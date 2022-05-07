#include "SmokeParticleSystem.h"

SmokeParticleSystem::SmokeParticleSystem(Geometry geometry, Material material, Vector3 spawnMin, Vector3 spawnMax) : ParticleSystem(geometry, material)
{
	for (int i = 0; i < 100; i++)
	{
		_particlePool.push_back(CreateParticle());
	}
	_timeUntilNextSpawn = 0.0f;

	_spawnLocation = Vector3();
	_spawnMax = spawnMax;
	_spawnMin = spawnMin;
	_windDirection = Vector3();
	_upwards = Vector3(0.0f, 2.0f, 0.0f);
}

void SmokeParticleSystem::SetSpawnLocation(Vector3 location)
{
	_spawnLocation = location;
}

void SmokeParticleSystem::SetWindDirection(Vector3 direction)
{
	_windDirection = direction;
}

void SmokeParticleSystem::Update(float deltaTime)
{
	if (_enabled)
	{
		if (_timeUntilNextSpawn <= 0.0f)
		{
			_timeUntilNextSpawn = 0.05f;
			Particle* particle = GetFirstInactive();

			if (particle == nullptr)
			{
				particle = CreateParticle();
				_particlePool.push_back(particle);
				Debug::GetInstance().WriteLine("Generated new Smoke Particle");
			}

			particle->GetRigidbody()->Reset();
			Vector3 offset = _spawnMax - _spawnMin;
			offset.x *= ((rand() % 1000) / 1000.0f);
			offset.y *= ((rand() % 1000) / 1000.0f);
			offset.z *= ((rand() % 1000) / 1000.0f);
			particle->GetGameObject()->GetTransform()->SetPosition(Vector3(_spawnLocation + _spawnMin + offset));
			particle->SetLife(5.0f);
			particle->SetActive(true);
		}
		_timeUntilNextSpawn -= deltaTime;
	}

	_forces += _windDirection;
	_forces += _upwards;

	for (Particle* particle : _particlePool)
	{
		Vector3 localForce = _forces;
		localForce += Vector3(0.5 * sinf(particle->GetLife() * 5.0f), 0.0f, 0.5 * cosf(particle->GetLife() * 5.0f));

		/*Vector3 forcesNormal = localForce.Normalised();
		Vector3 down = Vector3(0.0f, -1.0f, 0.0f);
		float dot = Vector3(0.0f, 1.0f, 0.0f).DotProduct(forcesNormal);

		Quaternion q;
		if (dot <= -1.0f || dot >= 1.0f)
		{

		}
		else
		{
			Vector3 a = down.CrossProduct(forcesNormal);
			q.i = a.x;
			q.j = a.y;
			q.k = a.z;
			q.r = sqrt(pow(down.Magnitude(), 2) * pow(forcesNormal.Magnitude(), 2)) + dot;
		}*/

		//particle->GetGameObject()->GetTransform()->SetRotation(q);
		particle->Update(deltaTime, localForce);
	}
	ParticleSystem::Update(deltaTime);
}

Particle* SmokeParticleSystem::CreateParticle()
{
	Particle* particle = ParticleSystem::CreateParticle();
	particle->GetGameObject()->GetTransform()->SetScale(0.01f, 0.01f, 0.01f);
	particle->GetRigidbody()->SetGravityLimit(-1.0f);
	return particle;
}
