#include "SphereCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"
#include "Rigidbody.h"

SphereCollider::SphereCollider(float radius)
{
	_radius = radius;
}

float SphereCollider::Centre(int axis)
{
	if (axis == 0)
		return _gameObject->GetTransform()->GetPosition().x;
	else if (axis == 1)
		return _gameObject->GetTransform()->GetPosition().y;
	else if (axis == 2)
		return _gameObject->GetTransform()->GetPosition().z;
	else
		return MININT;
}

bool SphereCollider::IntersectsVisit(Collider* other, float deltaTime)
{
	return other->Intersects(this, deltaTime);
}

bool SphereCollider::Intersects(SphereCollider* other, float deltaTime)
{
	Vector3 selfOffset = Vector3();
	if (_gameObject->GetComponent<Rigidbody>() != nullptr)
		selfOffset = _gameObject->GetComponent<Rigidbody>()->GetVelocity() * deltaTime;
	Vector3 otherOffset = Vector3();
	if (other->GetGameObject()->GetComponent<Rigidbody>() != nullptr)
		otherOffset = other->GetGameObject()->GetComponent<Rigidbody>()->GetVelocity() * deltaTime;

	Vector3 difference = (other->GetGameObject()->GetTransform()->GetPosition() + otherOffset) - (_gameObject->GetTransform()->GetPosition() + selfOffset);
	return difference.x * difference.x + difference.y * difference.y + difference.z * difference.z <= _radius + other->GetRadius();
}

bool SphereCollider::Intersects(AABBCollider* other, float deltaTime)
{
	return other->Intersects(this, deltaTime);
}

bool SphereCollider::Intersects(OBBCollider* other, float deltaTime)
{
	return other->Intersects(this, deltaTime);
}
