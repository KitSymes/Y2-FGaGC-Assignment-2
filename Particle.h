#pragma once
#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "Vector3.h"
#include "GameObject.h"
#include "Rigidbody.h"

class Particle
{
private:
	GameObject* _gameObject;
	Rigidbody* _rigidbody;

	float _life;
	bool _active;
public:
	Particle(Geometry geometry, Material material);
	~Particle();

	GameObject* GetGameObject();
	Rigidbody* GetRigidbody();
	bool IsActive();

	float GetLife();

	void SetActive(bool active);
	void SetLife(float life);

	void Update(float deltaTime, Vector3 forces);
	void Draw(ID3D11DeviceContext* pImmediateContext);
};
#endif
