#include "OBBCollider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include "Rigidbody.h"

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

bool OBBCollider::IntersectsLine(Vector3 start, Vector3 end)
{
	Vector3 direction = end - start;
	vector<Vector3> directionVerticies;
	directionVerticies.push_back(start);
	directionVerticies.push_back(end);

	// Seperating Axis Theorem
	Quaternion selfRotation = _gameObject->GetTransform()->GetRotation();

	vector<Vector3> selfVerticies;
	selfVerticies.push_back(selfRotation * _min + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * Vector3(_max.x, _min.y, _min.x) + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * Vector3(_min.x, _max.y, _min.x) + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * Vector3(_min.x, _min.y, _max.x) + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * _max + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * Vector3(_min.x, _max.y, _max.x) + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * Vector3(_max.x, _min.y, _max.x) + _gameObject->GetTransform()->GetPosition());
	selfVerticies.push_back(selfRotation * Vector3(_max.x, _max.y, _min.x) + _gameObject->GetTransform()->GetPosition());
	float selfMinBound, selfMaxBound, otherMinBound, otherMaxBound;

	// Check own face normals - Right, Up, Front
	Vector3 selfR = selfRotation * Vector3(1.0f, 0.0f, 0.0f);
	Vector3 selfU = selfRotation * Vector3(0.0f, 1.0f, 0.0f);
	Vector3 selfF = selfRotation * Vector3(0.0f, 0.0f, 1.0f);
	// Pairs checks
	// selfR X other<>
	Vector3 sRxoR = selfR.CrossProduct(direction);

	SeparatingAxisTest(sRxoR, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sRxoR, directionVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 sRxoU = selfR.CrossProduct(direction);

	SeparatingAxisTest(sRxoU, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sRxoU, directionVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 sRxoF = selfR.CrossProduct(direction);

	SeparatingAxisTest(sRxoF, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sRxoF, directionVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	// selfU X other<>
	Vector3 sRxo = selfR.CrossProduct(direction);

	SeparatingAxisTest(sRxo, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sRxo, directionVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 sUxoU = selfU.CrossProduct(direction);

	SeparatingAxisTest(sUxoU, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sUxoU, directionVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 sFxo = selfF.CrossProduct(direction);

	SeparatingAxisTest(sFxo, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(sFxo, directionVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	return true;
}

bool OBBCollider::IntersectsVisit(Collider* other, float deltaTime)
{
	return other->Intersects(this, deltaTime);
}

bool OBBCollider::Intersects(SphereCollider* other, float deltaTime)
{
	Vector3 selfOffset = Vector3();
	if (_gameObject->GetComponent<Rigidbody>() != nullptr)
		selfOffset = _gameObject->GetComponent<Rigidbody>()->GetVelocity() * deltaTime;
	Vector3 otherOffset = Vector3();
	if (other->GetGameObject()->GetComponent<Rigidbody>() != nullptr)
		otherOffset = other->GetGameObject()->GetComponent<Rigidbody>()->GetVelocity() * deltaTime;

	Vector3 localSphereCenter = (other->GetGameObject()->GetTransform()->GetPosition() + otherOffset) - (_gameObject->GetTransform()->GetPosition() + selfOffset);
	Vector3 closest = Vector3(max(_min.x, min(localSphereCenter.x, _max.x)),
		max(_min.y, min(localSphereCenter.y, _max.y)),
		max(_min.z, min(localSphereCenter.z, _max.z)));
	Vector3 offset = closest - localSphereCenter;
	Vector3 offsetNormalised = offset;
	offsetNormalised.Normalise();
	Vector3 translation = (other->GetRadius() - offset.Magnitude()) * offsetNormalised;

	return (translation.x <= 0.0f && translation.y <= 0.0f && translation.z <= 0.0f);
}

bool OBBCollider::Intersects(AABBCollider* other, float deltaTime)
{
	Vector3 selfOffset = Vector3();
	if (_gameObject->GetComponent<Rigidbody>() != nullptr)
		selfOffset = _gameObject->GetComponent<Rigidbody>()->GetVelocity() * deltaTime;
	Vector3 otherOffset = Vector3();
	if (other->GetGameObject()->GetComponent<Rigidbody>() != nullptr)
		otherOffset = other->GetGameObject()->GetComponent<Rigidbody>()->GetVelocity() * deltaTime;

	// Seperating Axis Theorem
	Quaternion selfRotation = _gameObject->GetTransform()->GetRotation();

	vector<Vector3> selfVerticies;
	selfVerticies.push_back(selfRotation * _min + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * Vector3(_max.x, _min.y, _min.x) + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * Vector3(_min.x, _max.y, _min.x) + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * Vector3(_min.x, _min.y, _max.x) + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * _max + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * Vector3(_min.x, _max.y, _max.x) + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * Vector3(_max.x, _min.y, _max.x) + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * Vector3(_max.x, _max.y, _min.x) + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);

	vector<Vector3> otherVerticies;
	otherVerticies.push_back(other->GetMin() + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(Vector3(other->GetMax().x, other->GetMin().y, other->GetMin().x) + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(Vector3(other->GetMin().x, other->GetMax().y, other->GetMin().x) + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(Vector3(other->GetMin().x, other->GetMin().y, other->GetMax().x) + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(other->GetMax() + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(Vector3(other->GetMin().x, other->GetMax().y, other->GetMax().x) + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(Vector3(other->GetMax().x, other->GetMin().y, other->GetMax().x) + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(Vector3(other->GetMax().x, other->GetMax().y, other->GetMin().x) + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);

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

	Vector3 otherR = Vector3(1.0f, 0.0f, 0.0f);

	SeparatingAxisTest(otherR, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(otherR, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 otherU = Vector3(0.0f, 1.0f, 0.0f);

	SeparatingAxisTest(otherU, selfVerticies, selfMinBound, selfMaxBound);
	SeparatingAxisTest(otherU, otherVerticies, otherMinBound, otherMaxBound);
	if (!Overlaps(selfMinBound, selfMaxBound, otherMinBound, otherMaxBound))
		return false;

	Vector3 otherF = Vector3(0.0f, 0.0f, 1.0f);

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

bool OBBCollider::Intersects(OBBCollider* other, float deltaTime)
{
	Vector3 selfOffset = Vector3();
	if (_gameObject->GetComponent<Rigidbody>() != nullptr)
		selfOffset = _gameObject->GetComponent<Rigidbody>()->GetVelocity() * deltaTime;
	Vector3 otherOffset = Vector3();
	if (other->GetGameObject()->GetComponent<Rigidbody>() != nullptr)
		otherOffset = other->GetGameObject()->GetComponent<Rigidbody>()->GetVelocity() * deltaTime;

	// Seperating Axis Theorem
	Quaternion selfRotation = _gameObject->GetTransform()->GetRotation();
	Quaternion otherRotation = other->GetGameObject()->GetTransform()->GetRotation();

	vector<Vector3> selfVerticies;
	selfVerticies.push_back(selfRotation * _min + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * Vector3(_max.x, _min.y, _min.x) + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * Vector3(_min.x, _max.y, _min.x) + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * Vector3(_min.x, _min.y, _max.x) + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * _max + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * Vector3(_min.x, _max.y, _max.x) + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * Vector3(_max.x, _min.y, _max.x) + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);
	selfVerticies.push_back(selfRotation * Vector3(_max.x, _max.y, _min.x) + _gameObject->GetTransform()->GetPosition() + selfOffset * deltaTime);

	vector<Vector3> otherVerticies;
	otherVerticies.push_back(otherRotation * other->GetMin() + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(otherRotation * Vector3(other->GetMax().x, other->GetMin().y, other->GetMin().x) + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(otherRotation * Vector3(other->GetMin().x, other->GetMax().y, other->GetMin().x) + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(otherRotation * Vector3(other->GetMin().x, other->GetMin().y, other->GetMax().x) + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(otherRotation * other->GetMax() + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(otherRotation * Vector3(other->GetMin().x, other->GetMax().y, other->GetMax().x) + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(otherRotation * Vector3(other->GetMax().x, other->GetMin().y, other->GetMax().x) + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);
	otherVerticies.push_back(otherRotation * Vector3(other->GetMax().x, other->GetMax().y, other->GetMin().x) + other->GetGameObject()->GetTransform()->GetPosition() + otherOffset * deltaTime);

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
