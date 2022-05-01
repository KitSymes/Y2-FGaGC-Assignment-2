#include "SphereCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"

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

bool SphereCollider::IntersectsVisit(Collider* collider)
{
	return collider->Intersects(this);
}

bool SphereCollider::Intersects(SphereCollider* collider)
{
	Vector3 difference = collider->GetGameObject()->GetTransform()->GetPosition() - _gameObject->GetTransform()->GetPosition();
	return difference.x * difference.x + difference.y * difference.y + difference.z * difference.z <= _radius + collider->GetRadius();
}

bool SphereCollider::Intersects(AABBCollider* collider)
{
	float dmin = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		if (Centre(i) < collider->Min(i))
			dmin += pow(Centre(i) - collider->Min(i), 2);
		else if (Centre(i) > collider->Max(i))
			dmin += pow(Centre(i) - collider->Max(i), 2);
	}
	return dmin <= pow(_radius, 2);
}

bool SphereCollider::Intersects(OBBCollider* other)
{
	return other->Intersects(this);
}
