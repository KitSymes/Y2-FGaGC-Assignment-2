#include "Octree.h"

Octree::Octree(Vector3 centre)
{
	_centre = centre;
	_leafObjects = vector<Collider*>();
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				_children[i][j][k] = nullptr;
}

Octree::~Octree()
{
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				if (_children[i][j][k] != nullptr)
					delete _children[i][j][k];

	_leafObjects.clear();
}

void Octree::Create(vector<Collider*> colliders, float sideLength)
{
	// This is a leaf because it only has 1 object
	// OR This is a leaf because the size length is <= 1
	if (colliders.size() == 1 || sideLength <= 1.0f)
	{
		for (Collider* go : colliders)
			_leafObjects.push_back(go);
		return;
	}

	vector<Collider*> temp[2][2][2];

	for (Collider* go : colliders)
	{
		if (go->IntersectsLine(_centre - Vector3(sideLength / 2.0f, 0.0f, 0.0f), _centre + Vector3(sideLength / 2.0f, 0.0f, 0.0f)))
		{
			_leafObjects.push_back(go);
			continue;
		}
		if (go->IntersectsLine(_centre - Vector3(0.0f, sideLength / 2.0f, 0.0f), _centre + Vector3(0.0f, sideLength / 2.0f, 0.0f)))
		{
			_leafObjects.push_back(go);
			continue;
		}
		if (go->IntersectsLine(_centre - Vector3(0.0f, 0.0f, sideLength / 2.0f), _centre + Vector3(0.0f, 0.0f, sideLength / 2.0f)))
		{
			_leafObjects.push_back(go);
			continue;
		}

		Vector3 position = go->GetGameObject()->GetTransform()->GetPosition();
		temp[(position.x > _centre.x)][(position.y > _centre.y)][(position.z > _centre.z)].push_back(go);
	}

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				if (temp[i][j][k].size() > 0)
				{
					_children[i][j][k] = new Octree(Vector3(((i * 2) - 1), ((j * 2) - 1), ((k * 2) - 1)) * (sideLength / 2.0f) + _centre);
					_children[i][j][k]->Create(temp[i][j][k], (sideLength / 2.0f));
					temp[i][j][k].clear();
				}
}

void Octree::DetectCollisions(float deltaTime, vector<Collider*> predecessors)
{
	for (Collider* col : _leafObjects)
		predecessors.push_back(col);

	if (predecessors.size() > 1)
		for (int i = 0; i < predecessors.size(); i++)
		{
			for (int j = i + 1; j < predecessors.size(); j++)
			{
				if (predecessors[i]->IntersectsVisit(predecessors[j], deltaTime))
				{
					//_colliders[i]->GetGameObject()->GetComponent<Rigidbody>()->SetAcceleration(Vector3());
					//_colliders[i]->GetGameObject()->GetComponent<Rigidbody>()->SetVelocity(Vector3());
					Rigidbody* rbI = predecessors[i]->GetGameObject()->GetComponent<Rigidbody>();
					Rigidbody* rbJ = predecessors[j]->GetGameObject()->GetComponent<Rigidbody>();

					if (rbI != nullptr) // This one has a RB
					{
						if (rbJ != nullptr)
						{
							// Pick the object with the higher momentum to act on
							if (rbI->GetVelocity().Magnitude() * rbI->GetMass() >= rbJ->GetVelocity().Magnitude() * rbJ->GetMass())
								rbI->CollidedWith(predecessors[j]);
							else
								rbJ->CollidedWith(predecessors[i]);
						}
						else
							rbI->CollidedWith(predecessors[j]);
					}
					else if (rbJ != nullptr) // Only the other one has an RB
						rbJ->CollidedWith(predecessors[i]);
				}
			}
		}

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				if (_children[i][j][k] != nullptr)
					_children[i][j][k]->DetectCollisions(deltaTime, predecessors);
}
