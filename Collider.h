#pragma once
#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "Component.h"

class SphereCollider;
class AABBCollider;
class OBBCollider;

class Collider :
	public Component
{
public:
	virtual bool IntersectsVisit(Collider* collider) { return false; };
	virtual bool Intersects(SphereCollider* collider) { return false; };
	virtual bool Intersects(AABBCollider* collider) { return false; };
	virtual bool Intersects(OBBCollider* collider) { return false; };
};
#endif
