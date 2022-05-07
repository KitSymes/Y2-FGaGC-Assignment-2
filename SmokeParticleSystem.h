#pragma once
#ifndef _SMOKE_PARTICLE_SYSTEM_H
#define _SMOKE_PARTICLE_SYSTEM_H

#include <stdlib.h>
#include "ParticleSystem.h"

class SmokeParticleSystem :
    public ParticleSystem
{
private:
    float _timeUntilNextSpawn;

    Vector3 _spawnLocation;
    Vector3 _spawnMin;
    Vector3 _spawnMax;
    Vector3 _windDirection;
    Vector3 _upwards;
public:
    SmokeParticleSystem(Geometry geometry, Material material, Vector3 spawnMin, Vector3 spawnMax);

    void SetSpawnLocation(Vector3 location);
    void SetWindDirection(Vector3 direction);

    virtual void Update(float deltaTime);
protected:
    virtual Particle* CreateParticle();
};
#endif
