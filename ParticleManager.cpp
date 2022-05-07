#include "ParticleManager.h"
#include "Application.h"

ParticleManager::ParticleManager()
{
    _nextID = 0;
    _systems = map<int, ParticleSystem*>();
}

ParticleManager::~ParticleManager()
{
    for (auto value : _systems)
        delete value.second;
    _systems.clear();
}

int ParticleManager::CreateSystem(ParticleSystem* system)
{
    int id = _nextID;
    _nextID++;
    _systems[id] = system;
    return id;
}

void ParticleManager::DeleteSystem(int key)
{
    delete _systems[key];
    _systems.erase(key);
}

void ParticleManager::Update(float deltaTime)
{
    for (auto value : _systems)
        value.second->Update(deltaTime);
}

void ParticleManager::Draw(ID3D11DeviceContext* pImmediateContext, ConstantBuffer* constantBuffer, ID3D11Buffer* pConstantBuffer)
{
    for (auto value : _systems)
        value.second->Draw(pImmediateContext, constantBuffer, pConstantBuffer);
}
