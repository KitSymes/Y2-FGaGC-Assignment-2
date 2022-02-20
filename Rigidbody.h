#pragma once
#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

#include "Component.h"
#include "Vector3.h"

using namespace DirectX;

class Rigidbody :
    public Component
{
private:
    bool _useConstantAcceleration;
    Vector3 _velocity;
    Vector3 _acceleration;

    float _mass;
    Vector3 _netForce;

    Vector3 _brake;

    const float _dragCoeficient = 0.5f;
    bool _laminar = true;
public:
    Rigidbody(bool useConstAccel, Vector3 initialVelocity, Vector3 initialAcceleration);

    void SetUsingConstantAcceleration(bool value);
    void SetAcceleration(Vector3 value);
    void SetVelocity(Vector3 value);

    void Update(float t);
    void UpdateNetForce();
    void UpdateAcceleration();

    Vector3 thrust;
};
#endif
