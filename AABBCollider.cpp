#include "AABBCollider.h"
#include "SphereCollider.h"

AABBCollider::AABBCollider(Vector3 min, Vector3 max)
{
	_min = min;
	_max = max;
}

float AABBCollider::Min(int axis)
{
	if (axis == 0)
		return _gameObject->GetTransform()->GetPosition().x + _min.x;
	else if (axis == 1)
		return _gameObject->GetTransform()->GetPosition().y + _min.y;
	else if (axis == 2)
		return _gameObject->GetTransform()->GetPosition().z + _min.z;
	else
		return MININT;
}

float AABBCollider::Max(int axis)
{
	if (axis == 0)
		return _gameObject->GetTransform()->GetPosition().x + _max.x;
	else if (axis == 1)
		return _gameObject->GetTransform()->GetPosition().y + _max.y;
	else if (axis == 2)
		return _gameObject->GetTransform()->GetPosition().z + _max.z;
	else
		return MAXINT;
}

bool AABBCollider::IntersectsVisit(Collider* collider)
{
	return collider->Intersects(this);
}

bool AABBCollider::Intersects(AABBCollider* collider)
{
	return false;
}

bool AABBCollider::Intersects(SphereCollider* collider)
{
	float dmin = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		if (collider->Centre(i) < Min(i))
			dmin += pow(collider->Centre(i) - Min(i), 2);
		else if (collider->Centre(i) > Max(i))
			dmin += pow(collider->Centre(i) - Max(i), 2);
	}
	return dmin <= pow(collider->GetRadius(), 2);
}