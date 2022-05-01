#pragma once
#ifndef _AABBCOLLIDER_H
#define _AABBCOLLIDER_H

#include "Collider.h"
#include "Vector3.h"
#include "GameObject.h"

class AABBCollider :
    public Collider
{
public:
	AABBCollider(Vector3 min, Vector3 max);

	float Min(int axis);
	float Max(int axis);

	Vector3 GetMin() { return _min; }
	Vector3 GetMax() { return _max; }

	bool IntersectsVisit(Collider* collider) override;
	bool Intersects(SphereCollider* collider) override;
	bool Intersects(AABBCollider* collider) override;
	bool Intersects(OBBCollider* collider) override;
private:
	Vector3 _min;
	Vector3 _max;
};
#endif
