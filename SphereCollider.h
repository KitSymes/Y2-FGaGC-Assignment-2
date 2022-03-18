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

	bool IntersectsVisit(Collider* collider) override; 
	bool Intersects(SphereCollider* collider) override;
	bool Intersects(AABBCollider* collider) override;
private:
	float _radius;

};
#endif
