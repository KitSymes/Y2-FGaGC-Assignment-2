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
	Quaternion selfRotation = _gameObject->GetTransform()->GetRotation();
	Quaternion otherRotation = other->GetGameObject()->GetTransform()->GetRotation();

	vector<Vector3> selfVerticies;
	selfVerticies.push_back(selfRotation * _min + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * Vector3(_max.x, _min.y, _min.x) + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * Vector3(_min.x, _max.y, _min.x) + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * Vector3(_min.x, _min.y, _max.x) + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * _max + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * Vector3(_min.x, _max.y, _max.x) + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * Vector3(_max.x, _min.y, _max.x) + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * Vector3(_max.x, _max.y, _min.x) + _gameObject->GetTransform()->GetPosition());

	vector<Vector3> otherVerticies;
	otherVerticies.push_back(otherRotation * other->GetMin() + other->GetGameObject()->GetTransform()->GetPosition());
	otherVerticies.push_back(otherRotation * Vector3(other->GetMax().x, other->GetMin().y, other->GetMin().x) + other->GetGameObject()->GetTransform()->GetPosition());
	otherVerticies.push_back(otherRotation * Vector3(other->GetMin().x, other->GetMax().y, other->GetMin().x) + other->GetGameObject()->GetTransform()->GetPosition());
	otherVerticies.push_back(otherRotation * Vector3(other->GetMin().x, other->GetMin().y, other->GetMax().x) + other->GetGameObject()->GetTransform()->GetPosition());
	otherVerticies.push_back(otherRotation * other->GetMax() + other->GetGameObject()->GetTransform()->GetPosition());
	otherVerticies.push_back(otherRotation * Vector3(other->GetMin().x, other->GetMax().y, other->GetMax().x) + other->GetGameObject()->GetTransform()->GetPosition());
	otherVerticies.push_back(otherRotation * Vector3(other->GetMax().x, other->GetMin().y, other->GetMax().x) + other->GetGameObject()->GetTransform()->GetPosition());
	otherVerticies.push_back(otherRotation * Vector3(other->GetMax().x, other->GetMax().y, other->GetMin().x) + other->GetGameObject()->GetTransform()->GetPosition());

	float selfMinBound, selfMaxBound, otherMinBound, otherMaxBound;

	// Check own face normals - Right, Up, Front
	Vector3 selfR = selfRotation * Vector3(1.0f, 0.0f, 0.0f);

	SeparatingAxisTest(selfR, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(selfR, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 selfU = selfRotation * Vector3(0.0f, 1.0f, 0.0f);

	SeparatingAxisTest(selfU, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(selfU, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 selfF = selfRotation * Vector3(0.0f, 0.0f, 1.0f);

	SeparatingAxisTest(selfF, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(selfF, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	// Check other face normals - Right, Up, Front

	Vector3 otherR = otherRotation * Vector3(1.0f, 0.0f, 0.0f);

	SeparatingAxisTest(otherR, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(otherR, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 otherU = otherRotation * Vector3(0.0f, 1.0f, 0.0f);

	SeparatingAxisTest(otherU, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(otherU, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 otherF = otherRotation * Vector3(0.0f, 0.0f, 1.0f);

	SeparatingAxisTest(otherF, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(otherF, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	// Pairs checks
	// selfR X other<>
	Vector3 sRxoR = selfR.CrossProduct(otherR);

	SeparatingAxisTest(sRxoR, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sRxoR, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 sRxoU = selfR.CrossProduct(otherU);

	SeparatingAxisTest(sRxoU, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sRxoU, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 sRxoF = selfR.CrossProduct(otherF);

	SeparatingAxisTest(sRxoF, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sRxoF, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	// selfU X other<>
	Vector3 sUxoR = selfU.CrossProduct(otherR);

	SeparatingAxisTest(sUxoR, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sUxoR, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 sUxoU = selfU.CrossProduct(otherU);

	SeparatingAxisTest(sUxoU, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sUxoU, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 sUxoF = selfU.CrossProduct(otherF);

	SeparatingAxisTest(sUxoF, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sUxoF, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	// selfF X other<>
	Vector3 sFxoR = selfF.CrossProduct(otherR);

	SeparatingAxisTest(sFxoR, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sFxoR, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 sFxoU = selfF.CrossProduct(otherU);

	SeparatingAxisTest(sFxoU, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sFxoU, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 sFxoF = selfF.CrossProduct(otherF);

	SeparatingAxisTest(sFxoF, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sFxoF, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	return true;
}
