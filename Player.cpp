#include "Player.h"

void Player::Initialize()
{
	//�V���O���g�����
	input = Input::GetInstance();
	

	player = { 640,360,0 };
}

void Player::Update()
{

}

void Player::Draw()
{
	
}

myMath::Vector3 Player::GetPlayerPos()
{
	return player;
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
