#include "Component.h"
#include "GameObject.h"

void Component::SetParent(GameObject* gameObject)
{
	_gameObject = gameObject;
}

void Component::Update(float deltaTime)
{
}

GameObject* Component::GetGameObject()
{
	return _gameObject;
}
