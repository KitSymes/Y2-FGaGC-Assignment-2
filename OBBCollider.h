#pragma once
#ifndef _OBBCOLLIDER_H
#define _OBBCOLLIDER_H

#include "Collider.h"
#include "Vector3.h"
#include "GameObject.h"

class OBBCollider :
	public Collider
{
public:
	OBBCollider(Vector3 min, Vector3 max);

	float Min(int axis);
	float Max(int axis);

	Vector3 GetMin() { return _min; }
	Vector3 GetMax() { return _max; }

	bool IntersectsVisit(Collider* other, float deltaTime) override;
	bool Intersects(SphereCollider* other, float deltaTime) override;
	bool Intersects(AABBCollider* other, float deltaTime) override;
	bool Intersects(OBBCollider* other, float deltaTime) override;
private:
	Vector3 _min;
	Vector3 _max;
};

static inline void SeparatingAxisTest(Vector3 axis, vector<Vector3>& verticies, float& minBound, float& maxBound)
{
	minBound = FLT_MAX;
	maxBound = -FLT_MAX;

	for (Vector3 vec : verticies)
	{
		float dotVal = vec.DotProduct(axis);
		if (dotVal < minBound) minBound = dotVal;
		if (dotVal > maxBound) maxBound = dotVal;
	}
}

static inline bool Overlaps(float selfMinBound, float selfMaxBound, float otherMinBound, float otherMaxBound)
{
	return ((selfMinBound <= otherMinBound && otherMinBound <= selfMaxBound)
		|| (otherMinBound <= selfMinBound && selfMinBound <= otherMaxBound));
}
#endif
