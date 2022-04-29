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

	bool IntersectsVisit(Collider* collider) override;
	bool Intersects(SphereCollider* collider) override;
	bool Intersects(AABBCollider* collider) override;
	bool Intersects(OBBCollider* collider) override;
private:
	Vector3 _min;
	Vector3 _max;
};
#endif
