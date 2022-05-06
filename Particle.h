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
	Rigidbody* _rigidBody;

	float _life;
	bool _active;
public:
	Particle(Geometry geometry, Material material, bool gravity);
	~Particle();

	GameObject* GetGameObject();
	Rigidbody* GetRigidbody();
	bool IsActive();

	void SetActive(bool active);

	void Update(float deltaTime);
	void Draw(ID3D11DeviceContext* pImmediateContext);
};
#endif
