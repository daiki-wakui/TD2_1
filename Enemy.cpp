#include "Enemy.h"
#include<cmath>

using namespace MathUtility;

void Enemy::Initialize(Model* model, const myMath::Vector3 position, uint32_t texture)
{
	assert(model);

	texture_ = texture;
	player = Player::GetInstance();
	model_ = model;
	score = Score::GetInstance();
	enemy = position;
	worldTransform.Initialize();
	worldTransform.translation_ = { position.x,position.y,position.z };

	//scoreBonus = 1.0;
}

void Enemy::Update()
{
	/*Move();*/
	/*Leave();*/
	MoveLimit();
	if (player->GetAttackFlag())
	{
		if ((1.0f + player->GetBombCharge()) * (1.0f + player->GetBombCharge()) >= (enemy.x - player->GetAttackWorldTransform().translation_.x) * (enemy.x - player->GetAttackWorldTransform().translation_.x) +

			(enemy.z - player->GetAttackWorldTransform().translation_.z) * (enemy.z - player->GetAttackWorldTransform().translation_.z))
		{
			isDead = true;
		}
	}

	if (player->GetSpeed() > 0.0f)
	{
		if ((1.0f + radius) * (1.0f + radius) >= (enemy.x - player->GetPlayerWorldTransform().translation_.x) * (enemy.x - player->GetPlayerWorldTransform().translation_.x)
			
			+ (enemy.z - player->GetPlayerWorldTransform().translation_.z) * (enemy.z - player->GetPlayerWorldTransform().translation_.z))
		{
			score->ScoreAdd();
			isDead = true;
		}
	}
	else
	{
		if ((1.0f + radius)* (1.0f + radius) >= (enemy.x - player->GetPlayerWorldTransform().translation_.x) * (enemy.x - player->GetPlayerWorldTransform().translation_.x) +
			
			(enemy.z - player->GetPlayerWorldTransform().translation_.z) * (enemy.z - player->GetPlayerWorldTransform().translation_.z))
		{
			//time-=3;
			isDead = true;
		}
	
	}

	MathUtility::MatrixCalculation(worldTransform);//行列の更新
	worldTransform.TransferMatrix();//行列の転送
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform, viewProjection, texture_);
}

void Enemy::Move()
{
	const float speed = 0.1f;

	enemyVec = { player->GetPlayerWorldTransform().translation_.x - enemy.x,
				 player->GetPlayerWorldTransform().translation_.y - enemy.y,
				 player->GetPlayerWorldTransform().translation_.z - enemy.z };
	normEnemyVec = enemyVec.normalization();

	angle = atan2(enemyVec.z, enemyVec.x);

	enemy.x += normEnemyVec.x * speed;
	enemy.y += normEnemyVec.y * speed;
	enemy.z += normEnemyVec.z * speed;
	worldTransform.translation_.x = enemy.x;
	worldTransform.translation_.y = enemy.y;
	worldTransform.translation_.z = enemy.z;

	worldTransform.rotation_.y = angle + myMath::AX_PI / 2;//敵の進行方向に向きを合わせる

	MathUtility::Matrix4Translation(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z);
}

void Enemy::Leave()
{
	const float speed = 3.0f;
	enemyRay = { (player->GetPlayerWorldTransform().translation_.x - enemy.x)*(player->GetPlayerWorldTransform().translation_.x - enemy.x),
				 (player->GetPlayerWorldTransform().translation_.y - enemy.y)* (player->GetPlayerWorldTransform().translation_.y - enemy.y),
				 (player->GetPlayerWorldTransform().translation_.z - enemy.z)* (player->GetPlayerWorldTransform().translation_.z - enemy.z) };
	normEnemyRay = enemyRay.normalization();
	distance = enemy.x- player->GetPlayerWorldTransform().translation_.x
		+ enemy.z  -player->GetPlayerWorldTransform().translation_.z ;
	if (distance <=1.0f)
	{
		enemy.x -= normEnemyRay.x * speed;
		enemy.z -= normEnemyRay.z * speed;
		speed - 1.0f;;
	}
	else
	{
		enemy.x = enemy.x;
		enemy.z = enemy.z;
	}
	/*if (normEnemyRay.z <= 0.5f)
	{
		
	}*/
	
	worldTransform.translation_.x = enemy.x;
	worldTransform.translation_.y = enemy.y;
	worldTransform.translation_.z = enemy.z;
	MathUtility::Matrix4Translation(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z);
}

void Enemy::MoveLimit()
{
	if (enemy.x <= -143)
	{
		wallHitIsDead = true;
	}
	else if (enemy.x >= 143)
	{
		wallHitIsDead = true;
	}
	if (enemy.z <= -93)
	{
		wallHitIsDead = true;
	}
	else if (enemy.z >= 93)
	{
		wallHitIsDead = true;
	}
}

WorldTransform Enemy::GetWorldTransform()
{
	return worldTransform;
}
