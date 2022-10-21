#include "EnemyBomb.h"

void EnemyBomb::Initialize(ViewProjection viewProjection, myMath::Vector3 position)
{
	player = Player::GetInstance();

	textureHandle = TextureManager::Load("bomuhei.jpg");
	model = Model::Create();
	worldTransform.Initialize();
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
	model->Draw(worldTransform,viewProjection,textureHandle);
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
			isDead = true;
		}
	}
	if ((1.0f + radius) * (1.0f + radius) >= (pos.x - player->GetPlayerWorldTransform().translation_.x) * (pos.x - player->GetPlayerWorldTransform().translation_.x) +
		(pos.z - player->GetPlayerWorldTransform().translation_.z) * (pos.z - player->GetPlayerWorldTransform().translation_.z))
	{
	
		isDead = true;
	}
}
WorldTransform EnemyBomb::GetWorldTransform()
{
	return worldTransform;
}
