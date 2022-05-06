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
}

void Rigidbody::SetInertiaTensor(float dx, float dy, float dz)
{
	ZeroMemory(&_inertiaTensor, sizeof(_inertiaTensor));

	float massMulti = (1.0f / 12.0f) * _mass;

	_inertiaTensor._11 = massMulti * (dy * dy + dz * dz);
	_inertiaTensor._22 = massMulti * (dx * dx + dz * dz);
	_inertiaTensor._33 = massMulti * (dx * dx + dy * dy);
}

void Rigidbody::SetMass(float mass)
{
	_mass = mass;
}

void Rigidbody::CalculateAngularAcceleration(float deltaTime)
{
	if (_torque.x == 0 && _torque.y == 0 && _torque.z == 0)
		return;
	XMMATRIX inverse = XMMatrixInverse(nullptr, XMLoadFloat3x3(&_inertiaTensor));
	XMStoreFloat3(&_angularAcceleration, XMVector3Transform(XMLoadFloat3(&_torque), inverse));
	_angularVelocity += Vector3(_angularAcceleration.x, _angularAcceleration.y, _angularAcceleration.z) * deltaTime;
	_torque = XMFLOAT3();
}

void Rigidbody::Update(float t)
{
	CalculateAngularAcceleration(t);

	// calculate net external force
	UpdateNetForce(t);

	// update acceleration of object using Newton’s second law of motion  
	// Assumption: net external force is constant between consecutive updates of object state
	if (!_useConstantAcceleration)
		UpdateAcceleration();

	// update world position and velocity of object
	// update world position of object by adding displacement to previously calculated position  
	_gameObject->GetTransform()->SetPosition(
		_gameObject->GetTransform()->GetPosition().x + _velocity.x * t + 0.5f * _acceleration.x * t * t,
		_gameObject->GetTransform()->GetPosition().y + _velocity.y * t + 0.5f * _acceleration.y * t * t,
		_gameObject->GetTransform()->GetPosition().z + _velocity.z * t + 0.5f * _acceleration.z * t * t);

	// update velocity of object by adding change relative to previously calculated velocity
	_velocity.x = _velocity.x + _acceleration.x * t;
	_velocity.y = _velocity.y + _acceleration.y * t;
	_velocity.z = _velocity.z + _acceleration.z * t;

	_angularVelocity *= pow(_angularDamping, t);
	Quaternion rot = _gameObject->GetTransform()->GetRotation();
	rot.addScaledVector(_angularVelocity, t);
	rot.normalise();
	_gameObject->GetTransform()->SetRotation(rot);

	_netForce = Vector3();
}

void Rigidbody::UpdateNetForce(float deltaTime)
{
	// Calculate Velocity Magnitude
	float velMag = _velocity.Magnitude();
	// Calculate Normalised (Unit Vector) Velocity
	Vector3 unitVel = _velocity;
	unitVel.Normalise();

	_netForce += thrust;

	if (_gameObject->GetTransform()->GetPosition().y > 0.5f)
		_netForce -= Vector3(0.0f, _mass * _gravity, 0.0f); // Gravity

	_netForce -= min(_mass * _gravity * _frictionCoefficient, velMag) * unitVel; // Friction

	Vector3 drag;
	if (_laminar)
	{
		// Calculate Drag
		float dragMag = 0.5f * _fluidDensityAir * _dragCoeficient * _referenceArea * velMag;

		drag = Vector3(dragMag * _velocity.x, dragMag * _velocity.y, dragMag * _velocity.z);
	}
	else // Turbulent
	{
		// Drag Force Magnitude
		float dragMag = 0.5f * _fluidDensityAir * _dragCoeficient * _referenceArea * velMag * velMag;

		// Calculate Drag
		drag = Vector3(dragMag * unitVel.x,
			dragMag * unitVel.y,
			dragMag * unitVel.z);

	}
	_netForce -= drag;

	float a = _gameObject->GetTransform()->GetPosition().y + _velocity.y * deltaTime;
	float b = 0.5f * (_netForce.y / _mass) * deltaTime * deltaTime;
	if (a + b < 0.5f)
	{
		_netForce.y = 0.0f;
		_velocity.y = 0.0f;

		Vector3 temp = _gameObject->GetTransform()->GetPosition();
		temp.y = 0.5f;
		_gameObject->GetTransform()->SetPosition(temp);

		//_netForce.y = ((0.5f - a) / (0.5f * deltaTime * deltaTime)) * _mass;

		//_netForce.y = ((0.5f - _gameObject->GetTransform()->GetPosition().y) / (0.5f * deltaTime * deltaTime)) * _mass;
		//_velocity.y = 0.0f;

		//_netForce.y = _velocity.y * -2.0f * _mass / deltaTime;
		//_netForce.y -= (_gameObject->GetTransform()->GetPosition().y - 0.5f) * _mass / deltaTime;
	}
}

void Rigidbody::UpdateAcceleration()
{
	// calculate acceleration caused by net external force
	_acceleration.x = _netForce.x / _mass;
	_acceleration.y = _netForce.y / _mass;
	_acceleration.z = _netForce.z / _mass;
}

void Rigidbody::AddForce(Vector3 force)
{
	_netForce += force;
}

void Rigidbody::AddForce(Vector3 force, Vector3 relativeLocation)
{
	AddForce(force);
	SetTorque(force, relativeLocation);
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
		// penetration depth stuff
		//{
		//	Vector3 normal = Vector3(); // Penetration Normal from this object's perspective
		//	float depth = normal.Magnitude(); // Penetration Depth
		//	normal.Normalise();
		//	Vector3 deltaPa = (otherRB->_mass / (_mass + otherRB->_mass)) * depth * normal;
		//	Vector3 deltaPb = -1 * (otherRB->_mass / (_mass + otherRB->_mass)) * depth * normal;
		//}

		// Modelling Collisions between two moving objects, considering elasticity
		//if (otherRB->_velocity.Magnitude() <= 0.001f)
		{
			//Vector3 momentum = _velocity * _mass;
			//Vector3 otherMomentum = otherRB->_velocity * otherRB->_mass;
			float elasticity = 0.5f;
			// mass gets ignored here because lecture notes just drop it? How do you work backwards with mass?
			Vector3 V1fminusV2f = -elasticity * (_velocity - otherRB->_velocity);
			Vector3 V1fplusV2f = _velocity + otherRB->_velocity; // Both should be multiplied by their mass,
			// but this causes issues as lecture notes just say assuming they have the same mass
			Vector3 v1f = (V1fplusV2f + V1fminusV2f) / 2.0f;
			Vector3 v2f = (V1fplusV2f - V1fminusV2f) / 2.0f;

			_velocity = v1f;
			otherRB->_velocity = v2f;
		}
		/*{
			// Using Kinetic Energy and Conservation of Momentum
			float M1 = _mass;
			float M2 = otherRB->_mass;
			Vector3 v1f = ((M1 - M2) / (M1 + M2)) * _velocity + ((2.0f * M2) / (M1 + M2)) * otherRB->_velocity;
			Vector3 v2f = ((M2 - M1) / (M1 + M2)) * otherRB->_velocity + ((2.0f * M1) / (M1 + M2)) * _velocity;

			_velocity = v1f;
			otherRB->_velocity = v2f;
		}*/
	}

}

void Rigidbody::Reset()
{
	_gameObject->GetTransform()->SetRotationDegrees(0.0f, 0.0f, 0.0f);
	_angularAcceleration = Vector3().ToXMFLOAT3();
	_angularVelocity = Vector3();
	SetVelocity(Vector3());
	SetAcceleration(Vector3());
}
