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

bool AABBCollider::IntersectsVisit(Collider* other)
{
	return other->Intersects(this);
}

bool AABBCollider::Intersects(AABBCollider* other)
{
	return (Min(0) <= other->Max(0) && Max(0) >= other->Min(0)) &&
		(Min(1) <= other->Max(1) && Max(1) >= other->Min(1)) &&
		(Min(2) <= other->Max(2) && Max(2) >= other->Min(2));
}

bool AABBCollider::Intersects(SphereCollider* other)
{
	float dmin = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		if (other->Centre(i) < Min(i))
			dmin += pow(other->Centre(i) - Min(i), 2);
		else if (other->Centre(i) > Max(i))
			dmin += pow(other->Centre(i) - Max(i), 2);
	}
	return dmin <= pow(other->GetRadius(), 2);
}