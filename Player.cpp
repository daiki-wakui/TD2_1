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
	debugText_ = DebugText::GetInstance();

	textureHundle = TextureManager::Load("sample.png");
	playerModel = Model::Create();
	playerWorldTransform.scale_ = { 2,2,2 };
	playerWorldTransform.Initialize();

	taleModel = Model::Create();
	taleWorldTransform.scale_;
	taleWorldTransform.Initialize();

	attackModel = Model::Create();
	attackWorldTransform.Initialize();

	player = { 0,0,-25 };
	front = { 0,0,-24 };
	angle = MathUtility::PI;
	attackTimer = 3;
	viewProjection.Initialize();
}

void Player::Update()
{
	Tale();//�K���̏���
	Rotation();//�v���C���[�̉�]����
	Move();//�v���C���[�̈ړ�����
	if (attackFlag == true)
	{
		Attack();//�U������
	}

	MathUtility::MatrixCalculation(playerWorldTransform);//�s��̍X�V
	playerWorldTransform.TransferMatrix();//�s��̓]��

	MathUtility::MatrixCalculation(taleWorldTransform);//�s��̍X�V
	taleWorldTransform.TransferMatrix();//�s��̓]��

	MathUtility::MatrixCalculation(attackWorldTransform);//�s��̍X�V
	attackWorldTransform.TransferMatrix();//�s��̓]��
}

void Player::Move()
{
	frontVec = front - player;
	normFrontVec = frontVec.normalization();

	MoveLimit();

	speed -= 0.01f;
	if (input->TriggerKey(DIK_SPACE))
	{
		speed = 0.065f * bombCharge;
		attackFlag = true;
	}
	if (speed <= 0.0f)
	{
		speed = 0.0f;
	}

	player.x -= normFrontVec.x * speed;
	player.z -= normFrontVec.z * speed;
	playerWorldTransform.translation_.x = player.x;
	playerWorldTransform.translation_.z = player.z;
}

void Player::MoveLimit()
{
	if (player.x <= -143)
	{
		player.x = -143;
	}
	else if (player.x >= 143)
	{
		player.x = 143;
	}
	if (player.z <= -93)
	{
		player.z = -93;
	}
	else if (player.z >= 93)
	{
		player.z = 93;
	}
}

void Player::Rotation()
{
	const float rotSpeed = 0.05f;

	if (input->TriggerKey(DIK_SPACE))
	{
		front.x = player.x + sinf(angle) * 2;
		front.z = player.z + cosf(angle) * 2;
	}
	taleWorldTransform.translation_.x = player.x + sinf(angle) * 2;
	taleWorldTransform.translation_.z = player.z + cosf(angle) * 2;

	attackWorldTransform.translation_.x = player.x + sinf(angle) * 8;
	attackWorldTransform.translation_.z = player.z + cosf(angle) * 8;

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
		angle = 0;//�I�[�o�[�t���[�������
	}
	else if (angle < 0)
	{
		angle = 2 * MathUtility::PI;//�I�[�o�[�t���[�������
	}

	playerWorldTransform.rotation_.y = angle;
	taleWorldTransform.rotation_.y = angle;
	attackWorldTransform.rotation_.y = angle;
}

void Player::Attack()
{
	attackTimer--;
	if (attackTimer <= 0)
	{
		bombCharge = 0.0f;
		attackTimer = 3;
		attackFlag = false;
	}
}

void Player::Tale()
{
	if (!input->PushKey(DIK_SPACE))
	{
		bombCharge += 0.2f;//�����Ă���Ԕ��e���`���[�W
	}

	if (bombCharge > 10.0f)
	{
		bombCharge = 10.0f;//�����ݒ�
	}

	taleWorldTransform.scale_.z = 1.0 + bombCharge / 4;//�K���̒���
	attackWorldTransform.scale_.x = 1.0 * bombCharge;
	attackWorldTransform.scale_.y = 1.0 * bombCharge;
	attackWorldTransform.scale_.z = 1.0 * bombCharge;
}

void Player::Draw(ViewProjection viewProjection)
{
	playerModel->Draw(playerWorldTransform, viewProjection, textureHundle);
	taleModel->Draw(taleWorldTransform, viewProjection, textureHundle);
	if (attackFlag == true)
	{
		attackModel->Draw(attackWorldTransform, viewProjection, textureHundle);
	}

	debugText_->SetPos(20, 60);
	debugText_->Printf("player.x %f,player.z %f", player.x, player.z);
}

WorldTransform Player::GetPlayerWorldTransform()
{
	return playerWorldTransform;
}

WorldTransform Player::GetTaleWorldTransform()
{
	return taleWorldTransform;
}

WorldTransform Player::GetAttackWorldTransform()
{
	return attackWorldTransform;
}

float Player::GetSpeed()
{
	return speed;
}

float Player::GetBombCharge()
{
	return bombCharge;
}

bool Player::GetAttackFlag()
{
	return attackFlag;
}

Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}