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

	bool Intersects(SphereCollider* collider) override;
	//bool Intersects(BoxCollider* collider) override;
private:
	float _radius;
};
#endif
