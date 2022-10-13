#include "Enemy.h"
#include "TextureManager.h"
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
	radius = 10.0f;


	worldTransform.Initialize();

	//scoreBonus = 1.0;
}

void Enemy::Update()
{
	Move();

	if (player->GetSpeed() > 0.0f)
	{
		if (radius * radius >= (enemy.x - player->GetWorldTransform().translation_.x) * (enemy.x - player->GetWorldTransform().translation_.x) + (enemy.y - player->GetWorldTransform().translation_.y) * (enemy.y - player->GetWorldTransform().translation_.y))
		{
			score->ScoreAdd();
			isDead = true;
		}
	}
	else
	{
		if (radius * radius >= (enemy.x - player->GetWorldTransform().translation_.x) * (enemy.x - player->GetWorldTransform().translation_.x) + (enemy.y - player->GetWorldTransform().translation_.y) * (enemy.y - player->GetWorldTransform().translation_.y))
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

bool Enemy::GetIsDead()
{
	return isDead;
}

void Enemy::Move()
{
	myMath::Vector3 trans;
	const float speed = 3.0f;

	enemyVec = { player->GetWorldTransform().translation_.x - enemy.x,
				 player->GetWorldTransform().translation_.y - enemy.y, 
				 player->GetWorldTransform().translation_.z - enemy.z };
	normEnemyVec = enemyVec.normalization();

	enemy.x += normEnemyVec.x * speed;
	enemy.y += normEnemyVec.y * speed;
}