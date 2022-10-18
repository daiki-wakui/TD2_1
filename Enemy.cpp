#include "Enemy.h"
#include "TextureManager.h"


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

	worldTransform.Initialize();

	//scoreBonus = 1.0;
}

void Enemy::Update()
{
	Move();

	if (player->GetSpeed() > 0.0f)
	{
		if (1.0f * radius >= (enemy.x - player->GetPlayerWorldTransform().translation_.x) * (enemy.x - player->GetPlayerWorldTransform().translation_.x)
			+(enemy.y - player->GetPlayerWorldTransform().translation_.y) * (enemy.y - player->GetPlayerWorldTransform().translation_.y)
			+ (enemy.z - player->GetPlayerWorldTransform().translation_.z) * (enemy.z - player->GetPlayerWorldTransform().translation_.z))
		{
			score->ScoreAdd();
			isDead = true;
		}
		
	}
	else
	{
		if ((1.0f + radius)* (1.0f + radius) >= (enemy.x - player->GetPlayerWorldTransform().translation_.x) * (enemy.x - player->GetPlayerWorldTransform().translation_.x) +
			(enemy.y - player->GetPlayerWorldTransform().translation_.y) * (enemy.y - player->GetPlayerWorldTransform().translation_.y) +
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

	enemy.x += normEnemyVec.x * speed;
	enemy.y += normEnemyVec.y * speed;
	enemy.z += normEnemyVec.z * speed;
	worldTransform.translation_.x = enemy.x;
	worldTransform.translation_.y = enemy.y;
	worldTransform.translation_.z = enemy.z;

	MathUtility::Matrix4Translation(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z);
}

WorldTransform Enemy::GetWorldTransform()
{
	return worldTransform;
}
