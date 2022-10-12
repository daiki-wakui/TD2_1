#include "Player.h"

void Player::Initialize()
{
	//ƒVƒ“ƒOƒ‹ƒgƒ“‘ã“ü
	input = Input::GetInstance();
	

	player = { 640,360,0 };
}

void Player::Update()
{

}

void Player::Draw()
{
	
}

Vector3 Player::GetPlayerPos()
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
