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
	//�V���O���g�����
	input = Input::GetInstance();
	
	textureHundle = TextureManager::Load("sample.png");
	playerModel = Model::Create();
	playerWorldTransform.scale_ = { 2,2,2 };
	playerWorldTransform.Initialize();

	taleModel= Model::Create();
	taleWorldTransform.scale_;
	taleWorldTransform.Initialize();

	player = { 0,0,0 };
	front = { 0,0,1 };
	viewProjection.Initialize();
}

void Player::Update()
{
	Bomb();
	Rotation();//�v���C���[�̉�]����
	Move();//�v���C���[�̈ړ�����

	MathUtility::MatrixCalculation(playerWorldTransform);//�s��̍X�V
	playerWorldTransform.TransferMatrix();//�s��̓]��

	MathUtility::MatrixCalculation(taleWorldTransform);//�s��̍X�V
	taleWorldTransform.TransferMatrix();//�s��̓]��
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

	playerWorldTransform.rotation_.y = angle;
	taleWorldTransform.rotation_.y = angle;
}

void Player::Bomb()
{
	if (!input->PushKey(DIK_SPACE))
	{
		bombCharge += 0.1f;
	}

	if (bombCharge > 10.0f)
	{
		bombCharge = 10.0f;
	}

	taleWorldTransform.scale_.z = 1.0 + bombCharge / 4;
}

void Player::Draw(ViewProjection viewProjection)
{
	playerModel->Draw(playerWorldTransform, viewProjection, textureHundle);
	taleModel->Draw(taleWorldTransform, viewProjection, textureHundle);
}

WorldTransform Player::GetWorldTransform()
{
	return playerWorldTransform;
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