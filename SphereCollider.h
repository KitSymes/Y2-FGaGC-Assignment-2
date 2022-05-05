#pragma once
#ifndef _SPHERE_COLLIDER_H
#define _SPHERE_COLLIDER_H

#include "Collider.h"
#include "Vector3.h"
#include "GameObject.h"

class SphereCollider :
    public Collider
{
public:
	SphereCollider(float radius);

	float GetRadius() { return _radius; };
	float Centre(int axis);

	bool IntersectsLine(Vector3 start, Vector3 end) override;

	bool IntersectsVisit(Collider* other, float deltaTime) override;
	bool Intersects(SphereCollider* other, float deltaTime) override;
	bool Intersects(AABBCollider* other, float deltaTime) override;
	bool Intersects(OBBCollider* other, float deltaTime) override;
private:
	float _radius;

};
#endif
