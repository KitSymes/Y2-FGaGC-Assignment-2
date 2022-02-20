#include "Rigidbody.h"
#include "GameObject.h"

Rigidbody::Rigidbody(bool useConstAccel, Vector3 initialVelocity, Vector3 initialAcceleration)
{
	_useConstantAcceleration = useConstAccel;
	_velocity = initialAcceleration;
	_acceleration = initialAcceleration;
	_mass = 1.0f;
	_netForce = Vector3();
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
