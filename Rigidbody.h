#pragma once
#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

#include "Component.h"
#include "Vector3.h"
#include "Collider.h"

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

    XMFLOAT3 _torque;
    XMFLOAT3X3 _inertiaTensor;
    XMFLOAT3 _angularAcceleration;
    Vector3 _angularVelocity;

    Vector3 _brake;

    const float _dragCoeficient = 1.05f;
    const float _angularDamping = 0.20f;
    bool _laminar = true;
public:
    Rigidbody(bool useConstAccel, Vector3 initialVelocity, Vector3 initialAcceleration);

    Vector3 GetVelocity() { return _velocity; }
    float GetMass() { return _mass; }

    void SetUsingConstantAcceleration(bool value);
    void SetAcceleration(Vector3 value);
    void SetVelocity(Vector3 value);
    void SetTorque(Vector3 force, Vector3 relativeLocation);
    void SetInertiaTensor(float dx, float dy, float dz);
    void SetMass(float mass);

    void CalculateAngularAcceleration();

    void Update(float t);
    void UpdateNetForce();
    void UpdateAcceleration();

    void CollidedWith(Collider* otherCollider);

    Vector3 thrust;
};
#endif
