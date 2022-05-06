#pragma once
#ifndef _PARTICLE_SYSTEM_H
#define _PARTICLE_SYSTEM_H

#include <vector>
#include "Particle.h"

class ParticleSystem
{
protected:
	vector<Particle*> _particlePool;

	Vector3 _forces;
public:
	ParticleSystem();
	~ParticleSystem();

	virtual void Update(float deltaTime) {}
	void Draw(ID3D11DeviceContext* pImmediateContext);
};
#endif
