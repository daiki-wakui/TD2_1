#include "EnemyStraight.h"

void EnemyStraight::Initialize(Model* model, ViewProjection viewProjection, myMath::Vector3 position, float enemyAngle)
{
	assert(model);
	player = Player::GetInstance();
	audioManager = AudioManager::GetInstance();

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
	worldTransform.rotation_.y = angle + myMath::AX_PI / 2;//敵の進行方向に向きを合わせる

	viewProjection.Initialize();
}

void EnemyStraight::Update(uint32_t& damageSE)
{
	moveVec = front - pos;
	normMoveVec = moveVec.normalization();

	Move();
	Limit();
	Collider(damageSE);

	MathUtility::MatrixCalculation(worldTransform);//行列の更新
	worldTransform.TransferMatrix();//行列の転送
}

void EnemyStraight::Move()
{
	pos.x -= normMoveVec.x * speed;
	pos.z -= normMoveVec.z * speed;
	worldTransform.translation_.x = pos.x;
	worldTransform.translation_.z = pos.z;
}

void EnemyStraight::Draw(ViewProjection viewProjection)
{
	//model->Draw(worldTransform, viewProjection, textureHundle);
	model_->Draw(worldTransform, viewProjection);
}

bool EnemyStraight::GetIsDead()
{
	return isDead;
}

bool EnemyStraight::GetWallHitIsDead()
{
	return wallHitIsDead;
}

bool EnemyStraight::GetPlayerHit()
{
	return playerHit;
}

void EnemyStraight::Limit()
{
	if (pos.x <= -142 || pos.x >= 142 || pos.z <= -92 || pos.z >= 92)
	{
		wallHitIsDead = true;
	}
}

void EnemyStraight::Collider(uint32_t damageSE)
{
	if (player->GetAttackFlag())
	{
		if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (pos.x - player->GetAttackWorldTransform().translation_.x) * (pos.x - player->GetAttackWorldTransform().translation_.x) + (pos.z - player->GetAttackWorldTransform().translation_.z) * (pos.z - player->GetAttackWorldTransform().translation_.z))
		{
			score->ScoreAdd();
			score->SetScoreAddTimer();
			score->SetKillFlag(true);
			audioManager->PlayWave(damageSE);
			isDead = true;
		}
	}


	if ((radius + 3.0f) * (radius + 3.0f) >= (pos.x - player->GetPlayerWorldTransform().translation_.x) * (pos.x - player->GetPlayerWorldTransform().translation_.x) +
		(pos.z - player->GetPlayerWorldTransform().translation_.z) * (pos.z - player->GetPlayerWorldTransform().translation_.z))
	{
		score->SetTimer(score->GetTimer() - 1);
		score->SetTimeSubTimer();
		score->SetDamageFlag(true);
		audioManager->PlayWave(damageSE);
		playerHit = true;
	}

}

WorldTransform EnemyStraight::GetWorldTransform()
{
	return worldTransform;
} 
