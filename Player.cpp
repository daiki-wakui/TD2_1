#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	delete model;
}

void Player::Initialize(ViewProjection viewProjection)
{
	//ƒVƒ“ƒOƒ‹ƒgƒ“‘ã“ü
	input = Input::GetInstance();
	
	textureHundle = TextureManager::Load("sample.png");
	model = Model::Create();
	worldTransform.Initialize();
	player = { 10,0,10 };
	front = { 10,0,11 };
	viewProjection.Initialize();

	
}

void Player::Update()
{
	Move();
}

void Player::Move()
{
	frontVec = front - player;
	normFrontVec = frontVec.normalization();
	worldTransform.translation_.x += normFrontVec.x * speed;
	worldTransform.translation_.z += normFrontVec.z * speed;

	if (!input->PushKey(DIK_SPACE))
	{
		bombCharge += 0.1f;
	}

	worldTransform.TransferMatrix();
}

void Player::Draw(ViewProjection viewProjection)
{
	model->Draw(worldTransform, viewProjection, textureHundle);
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