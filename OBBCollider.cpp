#include "OBBCollider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"

OBBCollider::OBBCollider(Vector3 min, Vector3 max)
{
	_min = min;
	_max = max;
}

float OBBCollider::Min(int axis)
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

float OBBCollider::Max(int axis)
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

bool OBBCollider::IntersectsVisit(Collider* other)
{
	return other->Intersects(this);
}

bool OBBCollider::Intersects(SphereCollider* other)
{
	/*
1
Use the OBB's orientation & translation information to inverse-transform the sphere's center into the box's local space.
localSphereCenter = box.InverseTransformPoint(sphere.worldCenter)
2
Find the closest point on the box to this point. Now that you're working on the box's local axes, this is as simple as a component-wise clamp on each axis:
closest.x = max(box.localMin.x, min(localSphereCenter.x, box.localMax.x));
... likewise for y, z
3
Take the offset vector from this closest point to the sphere center. If it's shorter than your radius, the sphere intersects the box.
The minimum translation vector (if the center is not in the box) is then this offset vector scaled to the remaining length:
translation = (sphere.radius - offset.length) * offset.normalized
If the sphere center is inside the box, then this displacement vector will be zero. You can clamp the point to the nearest face of the box, then form a penetration vector equal to the sphere center minus this closest face point. Your minimum translation vector is then:
translation = -1 * (sphere.radius + penetration.length) * penetration.normalized

Since we've computed these separations/penetrations in the box's local space, remember to transform them back to world space at the end to use them for moving objects.
	*/
	return false;
}

bool OBBCollider::Intersects(AABBCollider* other)
{
	return false;
}

bool OBBCollider::Intersects(OBBCollider* other)
{
	// Seperating Axis Theorem
	return false;
}
