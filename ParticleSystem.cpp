#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	_particlePool = vector<Particle*>();
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* particle : _particlePool)
		delete particle;
	_particlePool.clear();
}

void ParticleSystem::Draw(ID3D11DeviceContext* pImmediateContext)
{
	for (Particle* particle : _particlePool)
		particle->Draw(pImmediateContext);
}
