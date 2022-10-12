#include "Enemy.h"

void Enemy::Initialize(const Vector3 position)
{
	player = Player::GetInstance();
	score = Score::GetInstance();
	enemy = position;
	radius = 10.0f;
	//scoreBonus = 1.0;
}

void Enemy::Update()
{
	Move();

	if (player->GetSpeed() > 0.0f)
	{
		if (radius * radius >= (enemy.x - player->GetPlayerPos().x) * (enemy.x - player->GetPlayerPos().x) + (enemy.y - player->GetPlayerPos().y) * (enemy.y - player->GetPlayerPos().y))
		{
			score->ScoreAdd();
			isDead = true;
		}
	}
	else
	{
		if (radius * radius >= (enemy.x - player->GetPlayerPos().x) * (enemy.x - player->GetPlayerPos().x) + (enemy.y - player->GetPlayerPos().y) * (enemy.y - player->GetPlayerPos().y))
		{
			//time-=3;
			isDead = true;
		}
	}
}

void Enemy::Draw()
{

}

bool Enemy::GetIsDead()
{
	return isDead;
}

void Enemy::Move()
{
	const float speed = 3.0f;

	enemyVec = { player->GetPlayerPos() - enemy };
	normEnemyVec = enemyVec.normalization();

	enemy.x += normEnemyVec.x * speed;
	enemy.y += normEnemyVec.y * speed;
}