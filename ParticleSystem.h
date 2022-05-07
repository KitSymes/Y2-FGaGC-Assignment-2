#pragma once
#ifndef _PARTICLE_SYSTEM_H
#define _PARTICLE_SYSTEM_H

#include <vector>
#include "Particle.h"
#include "Debug.h"

struct ConstantBuffer;

class ParticleSystem
{
protected:
	vector<Particle*> _particlePool;
	Vector3 _forces;
	bool _enabled;
	Geometry _geometry;
	Material _material;
public:
	ParticleSystem(Geometry geometry, Material material);
	~ParticleSystem();

	bool IsEnabled();
	void Start();
	void Stop();

	Particle* GetFirstInactive();

	virtual void Update(float deltaTime);
	void Draw(ID3D11DeviceContext* pImmediateContext, ConstantBuffer* constantBuffer, ID3D11Buffer* pConstantBuffer);
protected:
	virtual Particle* CreateParticle();
};
#endif
