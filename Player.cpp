#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	delete model;
}

void Player::Initialize()
{
	//ƒVƒ“ƒOƒ‹ƒgƒ“‘ã“ü
	input = Input::GetInstance();
	
	textureHundle = TextureManager::Load("mario.jpg");
	model = Model::Create();
	worldTransform.Initialize();
	worldTransform.translation_ = { 10,0,10 };
	viewProjection.Initialize();

	
}

void Player::Update()
{

}

void Player::Draw()
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