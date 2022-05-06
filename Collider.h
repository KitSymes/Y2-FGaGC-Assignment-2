#pragma once
#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "Component.h"
#include "Vector3.h"

class SphereCollider;
class AABBCollider;
class OBBCollider;

class Collider :
	public Component
{
public:
	virtual bool IntersectsLine(Vector3 start, Vector3 end) { return false; }

	virtual bool IntersectsVisit(Collider* other, float deltaTime) { return false; }
	virtual bool Intersects(SphereCollider* other, float deltaTime) { return false; }
	virtual bool Intersects(AABBCollider* other, float deltaTime) { return false; }
	virtual bool Intersects(OBBCollider* other, float deltaTime) { return false; }
};
#endif
