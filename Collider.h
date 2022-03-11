#pragma once
#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "Component.h"

class SphereCollider;
//class BoxCollider;

class Collider :
	public Component
{
public:
	virtual bool Intersects(SphereCollider& collider) {};
	//virtual bool Intersects(BoxCollider* collider) = 0;
};
#endif
