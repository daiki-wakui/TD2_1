#include "EnemyStraight.h"

void EnemyStraight::Initialize(ViewProjection viewProjection,myMath::Vector3 position,float enemyAngle)
{
	player = Player::GetInstance();

	textureHundle = TextureManager::Load("mario.jpg");
	model = Model::Create();
	worldTransform.Initialize();

	pos = position;
	speed = 0.5f;
	angle = enemyAngle;

	front.x = pos.x + sinf(angle);
	front.z = pos.z + cosf(angle);

	viewProjection.Initialize();
}

void EnemyStraight::Update()
{
	moveVec = front - pos;
	normMoveVec = moveVec.normalization();

	Move();
	Limit();
	Collider();

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
	model->Draw(worldTransform, viewProjection, textureHundle);
}

bool EnemyStraight::GetIsDead()
{
	return isDead;
}

void EnemyStraight::Limit()
{
	if (pos.x <= -142 || pos.x >= 142 || pos.z <= -92 || pos.z >= 92)
	{
		isDead = true;
	}
}

void EnemyStraight::Collider()
{
	if (player->GetAttackFlag())
	{
		if ((1.0f + player->GetBombCharge()) * (1.0f + player->GetBombCharge()) >= (pos.x - player->GetAttackWorldTransform().translation_.x) * (pos.x - player->GetAttackWorldTransform().translation_.x) +(pos.z - player->GetAttackWorldTransform().translation_.z) * (pos.z - player->GetAttackWorldTransform().translation_.z))
		{
			isDead = true;
		}
	}
}

WorldTransform EnemyStraight::GetWorldTransform()
{
	return worldTransform;
}
