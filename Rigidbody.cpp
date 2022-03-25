#include "Rigidbody.h"
#include "GameObject.h"

Rigidbody::Rigidbody(bool useConstAccel, Vector3 initialVelocity, Vector3 initialAcceleration)
{
	_useConstantAcceleration = useConstAccel;
	_velocity = initialAcceleration;
	_acceleration = initialAcceleration;
	_mass = 1.0f;
	_netForce = Vector3();
	_torque = XMFLOAT3();
	SetInertiaTensor(1.0f, 1.0f, 1.0f);
	_angularVelocity = Vector3();
}

void Rigidbody::SetUsingConstantAcceleration(bool value)
{
	_useConstantAcceleration = value;
}

void Rigidbody::SetAcceleration(Vector3 value)
{
	_acceleration = value;
}

void Rigidbody::SetVelocity(Vector3 value)
{
	_velocity = value;
}

void Rigidbody::SetTorque(Vector3 force, Vector3 relativeLocation)
{
	_torque = relativeLocation.CrossProduct(force).ToXMFLOAT3();
	CalculateAngularAcceleration();
}

void Rigidbody::SetInertiaTensor(float dx, float dy, float dz)
{
	ZeroMemory(&_inertiaTensor, sizeof(_inertiaTensor));

	float massMulti = (1.0f / 12.0f) * _mass;

	_inertiaTensor._11 = massMulti * (dy*dy + dz*dz);
	_inertiaTensor._22 = massMulti * (dx*dx + dz*dz);
	_inertiaTensor._33 = massMulti * (dx*dx + dy*dy);
}

void Rigidbody::CalculateAngularAcceleration()
{
	XMMATRIX inverse = XMMatrixInverse(nullptr, XMLoadFloat3x3(&_inertiaTensor));
	XMStoreFloat3(&_angularAcceleration, XMVector3Transform(XMLoadFloat3(&_torque), inverse));
	_angularVelocity = Vector3(_angularAcceleration.x, _angularAcceleration.y, _angularAcceleration.z);
}

void Rigidbody::Update(float t)
{
	// calculate net external force
	UpdateNetForce();

	// update acceleration of object using Newton’s second law of motion  
	// Assumption: net external force is constant between consecutive updates of object state
	if (!_useConstantAcceleration)
		UpdateAcceleration();

	// update world position and velocity of object
	// update world position of object by adding displacement to previously calculated position  
	_gameObject->GetTransform()->SetPosition(
		_gameObject->GetTransform()->GetPosition().x + _velocity.x * t + 0.5 * _acceleration.x * t * t,
		_gameObject->GetTransform()->GetPosition().y + _velocity.y * t + 0.5 * _acceleration.y * t * t,
		_gameObject->GetTransform()->GetPosition().z + _velocity.z * t + 0.5 * _acceleration.z * t * t);

	// update velocity of object by adding change relative to previously calculated velocity
	_velocity.x = _velocity.x + _acceleration.x * t;
	_velocity.y = _velocity.y + _acceleration.y * t;
	_velocity.z = _velocity.z + _acceleration.z * t;

	_angularVelocity *= pow(_angularDamping, t);
	Quaternion rot = _gameObject->GetTransform()->GetRotation();
	rot.addScaledVector(_angularVelocity, t);
	rot.normalise();
	_gameObject->GetTransform()->SetRotation(rot);
}

void Rigidbody::UpdateNetForce()
{
	_netForce = Vector3();
	_netForce += thrust;
	_netForce += _brake;
	Vector3 drag;
	if (_laminar)
	{
		// Calculate Drag
		drag = Vector3(_dragCoeficient * _velocity.x,
			_dragCoeficient * _velocity.y,
			_dragCoeficient * _velocity.z);
	}
	else
	{
		// Calculate Velocity Magnitude
		float velMag = _velocity.Magnitude();
		// Calculate Normalised (Unit Vector) Velocity
		Vector3 unitVel = _velocity;
		unitVel.Normalise();

		// Drag Force Magnitude
		float dragMag = _dragCoeficient * velMag * velMag;

		// Calculate Drag
		drag = Vector3(dragMag * unitVel.x,
			dragMag * unitVel.y,
			dragMag * unitVel.z);

	}
	_netForce -= drag;
	//_netForce += Vector3(0.0f, _mass * 9.81f, 0.0f); // Gravity
}

void Rigidbody::UpdateAcceleration()
{
	// calculate acceleration caused by net external force
	_acceleration.x = _netForce.x / _mass;
	_acceleration.y = _netForce.y / _mass;
	_acceleration.z = _netForce.z / _mass;
}

void Rigidbody::CollidedWith(Collider* otherCollider)
{
	Collider* collider = _gameObject->GetComponent<Collider>();
	Rigidbody* otherRB = otherCollider->GetGameObject()->GetComponent<Rigidbody>();

	if (otherRB == nullptr)
	{
		// Other object is unable to move
		Vector3 normal = otherCollider->GetGameObject()->GetTransform()->GetPosition() - _gameObject->GetTransform()->GetPosition();
		normal.Normalise();
		normal *= -1;
		Vector3 result = _velocity - 2.0f * normal.DotProduct(_velocity) * normal;
		_velocity = result;
	}
	else
	{
		if (otherRB->_velocity.Magnitude() <= 0.01f)
		Vector3 momentum = _mass * _velocity;
		Vector3 otherMomentum = otherRB->_mass * otherRB->_velocity;

		Vector3 impulse = 0.01f * _netForce;
	}

}
