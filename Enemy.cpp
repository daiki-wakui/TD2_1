#include "Enemy.h"
#include<cmath>

using namespace MathUtility;

void Enemy::Initialize(Model* model, const myMath::Vector3 position)
{
	assert(model);

	player = Player::GetInstance();
	model_ = model;
	score = Score::GetInstance();
	audioManager = AudioManager::GetInstance();
	enemy = position;
	worldTransform.Initialize();
	worldTransform.scale_ = { 3,3,3 };
	worldTransform.translation_ = { position.x,position.y,position.z };

	//scoreBonus = 1.0;
}

void Enemy::Update(uint32_t& damageSE)
{
	Move();
	/*Leave();*/
	MoveLimit();
	if (player->GetAttackFlag())
	{
		if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (enemy.x - player->GetAttackWorldTransform().translation_.x) * (enemy.x - player->GetAttackWorldTransform().translation_.x) +

			(enemy.z - player->GetAttackWorldTransform().translation_.z) * (enemy.z - player->GetAttackWorldTransform().translation_.z))
		{
			score->ScoreAdd();
			score->SetScoreAddTimer();
			score->SetKillFlag(true);
			audioManager->PlayWave(damageSE);
			isDead = true;
		}
	}

	
	if ((radius + 3.0f) * (radius + 3.0f) >= (enemy.x - player->GetPlayerWorldTransform().translation_.x) * (enemy.x - player->GetPlayerWorldTransform().translation_.x)
			
		+ (enemy.z - player->GetPlayerWorldTransform().translation_.z) * (enemy.z - player->GetPlayerWorldTransform().translation_.z))
	{
		score->SetTimer(score->GetTimer() - 1);
		score->SetTimeSubTimer();
		score->SetDamageFlag(true);
		audioManager->PlayWave(damageSE);
		playerHit = true;
	}
	

	MathUtility::MatrixCalculation(worldTransform);//?s???̍X?V
	worldTransform.TransferMatrix();//?s???̓]??
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform, viewProjection);
}

void Enemy::Move()
{
	const float speed = 0.5f;

	enemyVec = { player->GetPlayerWorldTransform().translation_.x - enemy.x,
				 player->GetPlayerWorldTransform().translation_.y - enemy.y,
				 player->GetPlayerWorldTransform().translation_.z - enemy.z };
	normEnemyVec = enemyVec.normalization();

	angle = -(atan2(enemyVec.z, enemyVec.x) + myMath::AX_PI / 2);

	enemy.x += normEnemyVec.x * speed;
	enemy.y += normEnemyVec.y * speed;
	enemy.z += normEnemyVec.z * speed;
	worldTransform.translation_.x = enemy.x;
	worldTransform.translation_.y = enemy.y;
	worldTransform.translation_.z = enemy.z;

	worldTransform.rotation_.y = angle + myMath::AX_PI / 2;//?G?̐i?s?????Ɍ????????킹??

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
