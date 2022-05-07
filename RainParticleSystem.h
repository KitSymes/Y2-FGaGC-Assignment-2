#pragma once
#ifndef _RAIN_PARTICLE_SYSTEM_H
#define _RAIN_PARTICLE_SYSTEM_H

#include <stdlib.h>
#include "ParticleSystem.h"

class RainParticleSystem :
    public ParticleSystem
{
private:
    float _timeUntilNextSpawn;

    Vector3 _spawnMin;
    Vector3 _spawnMax;
    Vector3 _windDirection;
    Vector3 _gravity;
public:
    RainParticleSystem(Geometry geometry, Material material, Vector3 spawnMin, Vector3 spawnMax);

    void SetWindDirection(Vector3 direction);

    virtual void Update(float deltaTime);
protected:
    virtual Particle* CreateParticle();
};
#endif
