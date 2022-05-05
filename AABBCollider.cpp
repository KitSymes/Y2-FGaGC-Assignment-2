#include "AABBCollider.h"
#include "SphereCollider.h"
#include "OBBCollider.h"
#include "Rigidbody.h"

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

bool AABBCollider::IntersectsLine(Vector3 start, Vector3 end)
{
	if ((Min(0) <= start.x && Max(0) >= start.x) &&
		(Min(1)<= start.y && Max(1) >= start.y) &&
		(Min(2)<= start.z && Max(2) >= start.z))
		return true;

	if ((Min(0) <= end.x && Max(0) >= end.x) &&
		(Min(1) <= end.y && Max(1) >= end.y) &&
		(Min(2) <= end.z && Max(2) >= end.z))
		return true;

	// Closest point on the line to position (includes past start and end point), but that shouldn't matter
	Vector3 AB = end - start;
	Vector3 AP = _gameObject->GetTransform()->GetPosition() - start;
	float lengthSqrAB = AB.x * AB.x + AB.y * AB.y;
	float t = (AP.x * AB.x + AP.y * AB.y) / lengthSqrAB;
	if (t < 0)
		t = 0;
	if (t > 1)
		t = 1;
	Vector3 closest = start + t * AB;

	return ((Min(0) <= closest.x && Max(0) >= closest.x) &&
		(Min(1) <= closest.y && Max(1) >= closest.y) &&
		(Min(2) <= closest.z && Max(2) >= closest.z));
}

bool AABBCollider::IntersectsVisit(Collider* other, float deltaTime)
{
	return other->Intersects(this, deltaTime);
}

bool AABBCollider::Intersects(SphereCollider* other, float deltaTime)
{
	Vector3 selfOffset = Vector3();
	if (_gameObject->GetComponent<Rigidbody>() != nullptr)
		selfOffset = _gameObject->GetComponent<Rigidbody>()->GetVelocity() * deltaTime;
	Vector3 otherOffset = Vector3();
	if (other->GetGameObject()->GetComponent<Rigidbody>() != nullptr)
		otherOffset = other->GetGameObject()->GetComponent<Rigidbody>()->GetVelocity() * deltaTime;

	vector<float> selfOffsetVec;
	selfOffsetVec.push_back(selfOffset.x);
	selfOffsetVec.push_back(selfOffset.y);
	selfOffsetVec.push_back(selfOffset.z);
	vector<float> otherOffsetVec;
	otherOffsetVec.push_back(otherOffset.x);
	otherOffsetVec.push_back(otherOffset.y);
	otherOffsetVec.push_back(otherOffset.z);

	float dmin = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		if (other->Centre(i) + otherOffsetVec[i] < Min(i) + selfOffsetVec[i])
			dmin += pow(other->Centre(i) + otherOffsetVec[i] - Min(i) + selfOffsetVec[i], 2);
		else if (other->Centre(i) + otherOffsetVec[i] > Max(i) + selfOffsetVec[i])
			dmin += pow(other->Centre(i) + otherOffsetVec[i] - Max(i) + selfOffsetVec[i], 2);
	}
	return dmin <= pow(other->GetRadius(), 2);
}

bool AABBCollider::Intersects(AABBCollider* other, float deltaTime)
{
	Vector3 selfOffset = Vector3();
	if (_gameObject->GetComponent<Rigidbody>() != nullptr)
		selfOffset = _gameObject->GetComponent<Rigidbody>()->GetVelocity() * deltaTime;
	Vector3 otherOffset = Vector3();
	if (other->GetGameObject()->GetComponent<Rigidbody>() != nullptr)
		otherOffset = other->GetGameObject()->GetComponent<Rigidbody>()->GetVelocity() * deltaTime;

	return 
		(Min(0) + selfOffset.x <= other->Max(0) + otherOffset.x && Max(0) + selfOffset.x >= other->Min(0) + otherOffset.x) &&
		(Min(1) + selfOffset.y <= other->Max(1) + otherOffset.y && Max(1) + selfOffset.y >= other->Min(1) + otherOffset.y) &&
		(Min(2) + selfOffset.z <= other->Max(2) + otherOffset.z && Max(2) + selfOffset.z >= other->Min(2) + otherOffset.z);
}

bool AABBCollider::Intersects(OBBCollider* other, float deltaTime)
{
	return other->Intersects(this, deltaTime);
}
