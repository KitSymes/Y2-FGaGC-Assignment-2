#pragma once
#ifndef _PARTICLE_MANAGER_H
#define _PARTICLE_MANAGER_H

#include <map>
#include "ParticleSystem.h"

class ParticleManager
{
private:
	int _nextID;
	map<int, ParticleSystem*> _systems;
public:
	ParticleManager();
	~ParticleManager();

	int CreateSystem(ParticleSystem* system);
	template<typename T>
	T* GetSystem(int key)
	{
		if (T* v = dynamic_cast<T*>(_systems[key]))
			return v;
		return nullptr;
	}
	void DeleteSystem(int key);

	void Update(float deltaTime);
	void Draw(ID3D11DeviceContext* pImmediateContext, ConstantBuffer* constantBuffer, ID3D11Buffer* pConstantBuffer);
};
#endif
