#include "EnemyCircle.h"
void EnemyCircle::Initialize(Model* model,ViewProjection viewProjection, myMath::Vector3 position, float enemyAngle)
{
	assert(model);
	player = Player::GetInstance();

	textureHundle = TextureManager::Load("mario.jpg");
	model_ = model;
	worldTransform.scale_ = { 3,3,3 };
	worldTransform.Initialize();
	score = Score::GetInstance();

	pos = position;
	speed = 0.5f;
	angle = enemyAngle;

	front.x = pos.x + sinf(angle);
	front.z = pos.z + cosf(angle);
	worldTransform.rotation_.y = -(angle + myMath::AX_PI / 2);//敵の進行方向に向きを合わせる

	viewProjection.Initialize();
}

void EnemyCircle::Update()
{
	if (angle > 2 * MathUtility::PI)
	{
		angle = 0;
	}
	else if (angle < 0)
	{
		angle = 2 * MathUtility::PI;
	}
	else
	{
		angle += 0.02f;
	}
	worldTransform.rotation_.y = -(angle + myMath::AX_PI / 2);//敵の進行方向に向きを合わせる

	Move();
	Limit();
	Collider();

	MathUtility::MatrixCalculation(worldTransform);//行列の更新
	worldTransform.TransferMatrix();//行列の転送
}

void EnemyCircle::Move()
{
	/*pos = { 5 ,0,5 };*/
	moveVec = { 0 - pos.x,0,0 - pos.z };
	radian = sqrt((pos.x * pos.x) + (pos.z * pos.z));

	pos.x = cos(angle) * radian;
	pos.z = sin(angle) * radian;

	worldTransform.translation_.x = pos.x;
	worldTransform.translation_.y = pos.y;
	worldTransform.translation_.z = pos.z;
}

void EnemyCircle::Draw(ViewProjection viewProjection)
{
	//model->Draw(worldTransform,viewProjection,textureHundle);
	model_->Draw(worldTransform, viewProjection);
}

bool EnemyCircle::GetIsDead()
{
	return isDead;
}

void EnemyCircle::Limit()
{
	if (pos.x <= -142 || pos.x >= 142 || pos.z <= -92 || pos.z >= 92)
	{
		isDead = true;
	}
}

void EnemyCircle::Collider()
{
	if (player->GetAttackFlag())
	{
		if ((1.0f + player->GetBombCharge()) * (1.0f + player->GetBombCharge()) >= (pos.x - player->GetAttackWorldTransform().translation_.x) * (pos.x - player->GetAttackWorldTransform().translation_.x) + (pos.z - player->GetAttackWorldTransform().translation_.z) * (pos.z - player->GetAttackWorldTransform().translation_.z))
		{
			isDead = true;
		}
	}
	if (player->GetSpeed() >= 0.0f)
	{
		if ((1.0f + radius) * (1.0f + radius) >= (pos.x - player->GetPlayerWorldTransform().translation_.x) * (pos.x - player->GetPlayerWorldTransform().translation_.x) +
			(pos.z - player->GetPlayerWorldTransform().translation_.z) * (pos.z - player->GetPlayerWorldTransform().translation_.z))
		{
			score->ScoreAdd();
			isDead = true;
		}
	}
	else
	{
		if ((1.0f + radius) * (1.0f + radius) >= (pos.x - player->GetPlayerWorldTransform().translation_.x) * (pos.x - player->GetPlayerWorldTransform().translation_.x) +
			(pos.z - player->GetPlayerWorldTransform().translation_.z) * (pos.z - player->GetPlayerWorldTransform().translation_.z))
		{
			//time-=3;
			isDead = true;
		}

	}
}

WorldTransform EnemyCircle::GetWorldTransform()
{
	return worldTransform;
}
