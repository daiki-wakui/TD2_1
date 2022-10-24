#include "EnemyBomb.h"

void EnemyBomb::Initialize(Model* model, ViewProjection viewProjection, myMath::Vector3 position)
{
	assert(model);
	player = Player::GetInstance();

	model_ = model;
	worldTransform.Initialize();
	worldTransform.scale_ = { 3,3,3 };

	score = Score::GetInstance();

	pos = position;
	speed = 0.5f;

	viewProjection.Initialize();
}

void EnemyBomb::Update()
{
	Move();
	Limit();
	Collider();
	Detonation();
	MathUtility::MatrixCalculation(worldTransform);//行列の更新
	worldTransform.TransferMatrix();//行列の転送
}

void EnemyBomb::Move()
{
	worldTransform.translation_.x = pos.x;
	worldTransform.translation_.y = pos.y;
	worldTransform.translation_.z = pos.z;

}

void EnemyBomb::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform,viewProjection);
}

bool EnemyBomb::GetIsDead()
{
	return isDead;
}

void EnemyBomb::Limit()
{
	if (pos.x <= -142 || pos.x >= 142 || pos.z <= -92 || pos.z >= 92)
	{
		isDead = true;
	}
}

void EnemyBomb::Collider()
{
	if (player->GetAttackFlag())
	{
		if ((1.0f + player->GetBombCharge()) * (1.0f + player->GetBombCharge()) >= (pos.x - player->GetAttackWorldTransform().translation_.x) * (pos.x - player->GetAttackWorldTransform().translation_.x) + (pos.z - player->GetAttackWorldTransform().translation_.z) * (pos.z - player->GetAttackWorldTransform().translation_.z))
		{
			
			isCollision = true;
		}
	}
	if ((1.0f + radius) * (1.0f + radius) >= (pos.x - player->GetPlayerWorldTransform().translation_.x) * (pos.x - player->GetPlayerWorldTransform().translation_.x) +
		(pos.z - player->GetPlayerWorldTransform().translation_.z) * (pos.z - player->GetPlayerWorldTransform().translation_.z))
	{
	
		isDead = true;
	}
}
void EnemyBomb::Detonation()
{
	if (isCollision == true)
	{
		bombTimer--;
		worldTransform.scale_ = { 10,10,10 };
	}
	if (bombTimer <= 0)
	{
		isBomber = true;
		bombTimer = 180;
		isCollision = false;
	}
	if (isBomber == true)
	{
		if ((1.0f + player->GetBombCharge()) * (1.0f + player->GetBombCharge()) >= (pos.x - player->GetAttackWorldTransform().translation_.x) * (pos.x - player->GetAttackWorldTransform().translation_.x) + (pos.z - player->GetAttackWorldTransform().translation_.z) * (pos.z - player->GetAttackWorldTransform().translation_.z))
		{
			isCollision = true;
		}
		score->ScoreAdd();
		isDead = true;
	}
}
WorldTransform EnemyBomb::GetWorldTransform()
{
	return worldTransform;
}
