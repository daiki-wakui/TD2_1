#include "Player.h"

void Player::Initialize()
{
	//�V���O���g�����
	input = Input::GetInstance();
	mesh = Mesh::GetInstance();

	player = { 640,360,0 };
}

void Player::Update()
{

}

void Player::Draw()
{
	mesh->DrawEllipse(player.x, player.y, 10.0f, 10.0f, 0.0f, { 255,255,255,255 }, true);
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
