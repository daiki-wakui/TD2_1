#include "EnemyStraight.h"

void EnemyStraight::Initialize(ViewProjection viewProjection,myMath::Vector3 position,float enemyAngle)
{
	textureHundle = TextureManager::Load("mario.jpg");
	model = Model::Create();
	worldTransform.Initialize();

	pos = position;
	speed = 0.1f;
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

WorldTransform EnemyStraight::GetWorldTransform()
{
	return worldTransform;
}
