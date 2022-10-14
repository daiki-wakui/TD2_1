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
	playerWorldTransform.scale_ = { 2,2,2 };
	playerWorldTransform.Initialize();

	taleModel = Model::Create();
	taleWorldTransform.scale_;
	taleWorldTransform.Initialize();

	player = { 0,0,0 };
	front = { 0,0,1 };
	angle = MathUtility::PI;
	viewProjection.Initialize();
}

void Player::Update()
{
	Tale();
	Rotation();//プレイヤーの回転処理
	Move();//プレイヤーの移動処理

	MathUtility::MatrixCalculation(playerWorldTransform);//行列の更新
	playerWorldTransform.TransferMatrix();//行列の転送

	MathUtility::MatrixCalculation(taleWorldTransform);//行列の更新
	taleWorldTransform.TransferMatrix();//行列の転送
}

void Player::Move()
{
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

	player.x -= normFrontVec.x * speed;
	player.z -= normFrontVec.z * speed;
	playerWorldTransform.translation_.x = player.x;
	playerWorldTransform.translation_.z = player.z;

	front.x -= normFrontVec.x * speed;
	front.z -= normFrontVec.z * speed;

	frontPosition.x -= normFrontVec.x * speed;
	frontPosition.z -= normFrontVec.z * speed;
	taleWorldTransform.translation_.x = frontPosition.x;
	taleWorldTransform.translation_.z = frontPosition.z;
}

void Player::Rotation()
{
	const float rotSpeed = 0.05f;

	if (input->TriggerKey(DIK_SPACE))
	{
		front.x = player.x + sinf(angle) * 2;
		front.z = player.z + cosf(angle) * 2;
	}
	frontPosition.x = player.x + sinf(angle) * 2;
	frontPosition.z = player.z + cosf(angle) * 2;

	if (input->PushKey(DIK_A))
	{
		angle -= rotSpeed;
	}

	if (input->PushKey(DIK_D))
	{
		angle += rotSpeed;
	}

	if (angle > 2 * MathUtility::PI)
	{
		angle = 0;//オーバーフロー回避処理
	}
	else if (angle < 0)
	{
		angle = 2 * MathUtility::PI;//オーバーフロー回避処理
	}

	playerWorldTransform.rotation_.y = angle;
	taleWorldTransform.rotation_.y = angle;
}

void Player::Tale()
{
	if (!   input->PushKey(DIK_SPACE))
	{
		bombCharge += 0.2f;//離している間爆弾をチャージ
	}

	if (bombCharge > 10.0f)
	{
		bombCharge = 10.0f;//上限を設定
	}

	taleWorldTransform.scale_.z = 1.0 + bombCharge / 4;//尻尾の長さ
}

void Player::Draw(ViewProjection viewProjection)
{
	playerModel->Draw(playerWorldTransform, viewProjection, textureHundle);
	taleModel->Draw(taleWorldTransform, viewProjection, textureHundle);
}

WorldTransform Player::GetPlayerWorldTransform()
{
	return playerWorldTransform;
}

WorldTransform Player::GetTaleWorldTransform()
{
	return taleWorldTransform;
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