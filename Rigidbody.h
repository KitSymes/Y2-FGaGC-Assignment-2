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

    const float _dragCoeficient = 1.05f;
    const float _angularDamping = 0.20f;
    const float _gravity = 9.81f;
    const float _frictionCoefficient = 0.5f;
    const float _referenceArea = 1.0f;
    const float _fluidDensityAir = 1.225;

    bool _laminar = true;
    bool _useGravity = true;
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
    void SetGravity(bool gravity);

    void CalculateAngularAcceleration(float deltaTime);

    void Update(float deltaTime);
    void UpdateNetForce(float deltaTime);
    void UpdateAcceleration();

    void AddForce(Vector3 force);
    void AddForce(Vector3 force, Vector3 relativeLocation);
    void CollidedWith(Collider* otherCollider);

    void Reset();

    Vector3 thrust;
};
#endif
