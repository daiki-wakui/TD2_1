#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	delete playerModel;
}

void Player::Initialize(ViewProjection viewProjection)
{
	//シングルトン代入
	input = Input::GetInstance();
	
	textureHundle = TextureManager::Load("sample.png");
	playerModel = Model::Create();
	worldTransform.scale_ = { 2,2,2 };
	worldTransform.Initialize();
	player = { 10,0,10 };
	front = { 10,0,11 };
	viewProjection.Initialize();
}

void Player::Update()
{
	Bomb();
	Rotation();//プレイヤーの回転処理
	Move();//プレイヤーの移動処理

	MathUtility::MatrixCalculation(worldTransform);//行列の更新
	worldTransform.TransferMatrix();//行列の転送
}

void Player::Move()
{
	myMath::Vector3 trans;
	frontVec = front - player;
	normFrontVec = frontVec.normalization();

	speed -= 0.01f;
	if (input->TriggerKey(DIK_SPACE))
	{
		speed += 0.06f * bombCharge;
		bombCharge = 0.0f;
	}
	if (speed <= 0.0f)
	{
		speed = 0.0f;
	}

	trans.x -= normFrontVec.x * speed;
	trans.z -= normFrontVec.z * speed;
	worldTransform.translation_.x += trans.x;
	worldTransform.translation_.z += trans.z;
}

void Player::Rotation()
{
	const float rotSpeed = 0.05f;

	if (input->TriggerKey(DIK_SPACE))
	{
		front.x = player.x + sinf(angle);
		front.z = player.z + cosf(angle);
	}

	if (input->PushKey(DIK_A))
	{
		angle -= rotSpeed;
	}

	if (input->PushKey(DIK_D))
	{
		angle += rotSpeed;
	}

	worldTransform.rotation_.y = angle;
}

void Player::Bomb()
{
	if (!input->PushKey(DIK_SPACE))
	{
		bombCharge += 0.2f;
	}

	if (bombCharge > 10.0f)
	{
		bombCharge = 10.0f;
	}
}

void Player::Draw(ViewProjection viewProjection)
{
	playerModel->Draw(worldTransform, viewProjection, textureHundle);
	
}

WorldTransform Player::GetWorldTransform()
{
	return worldTransform;
}

float Player::GetSpeed()
{
	return speed;
}

Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}