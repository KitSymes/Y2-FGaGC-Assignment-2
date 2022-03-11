#include "SphereCollider.h"

SphereCollider::SphereCollider(float radius)
{
	_radius = radius;
}

bool SphereCollider::Intersects(SphereCollider* collider)
{
	Vector3 difference = collider->GetGameObject()->GetTransform()->GetPosition() - _gameObject->GetTransform()->GetPosition();
	return difference.x * difference.x + difference.y * difference.y + difference.z * difference.z > _radius + collider->GetRadius();
}

/*bool SphereCollider::Intersects(BoxCollider* collider)
{
	return false;
}*/
