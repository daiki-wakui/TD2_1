#include "Collider.h"

void Collider::CtoCCollision(const myMath::Vector3 v1, WorldTransform worldTransform, const float radius)
{
	 std::make_unique<Enemy>();
	if ((1.0f + radius) * (1.0f + radius) >= (v1.x - worldTransform.translation_.x) * (v1.x - worldTransform.translation_.x)
		+ (v1.y - worldTransform.translation_.y) * (v1.y - worldTransform.translation_.y)
		+ (v1.z - worldTransform.translation_.z) * (v1.z - worldTransform.translation_.z))
	{
		
		enemy->SetIsDead(true);
		
	}
}