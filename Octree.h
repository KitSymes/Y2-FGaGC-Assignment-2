#pragma once
#ifndef _OCTREE_H
#define _OCTREE_H

#include <vector>
#include "GameObject.h"
#include "Collider.h"
#include "Rigidbody.h"

class Octree
{
private:
	Octree* _children[2][2][2];
	vector<Collider*> _leafObjects;
	Vector3 _centre;
public:
	Octree(Vector3 centre);
	~Octree();

	void Create(vector<Collider*> colliders, float sideLength);
	void DetectCollisions(float deltaTime, vector<Collider*> predecessors);
};
#endif
