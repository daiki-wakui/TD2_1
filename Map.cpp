#include "Map.h"
#include "TextureManager.h"

Map::Map()
{
}

Map::~Map()
{
	delete backSprite;
}

void Map::Initialize(ViewProjection viewProjection)
{
	player = Player::GetInstance();
	player->Initialize(viewProjection);

	backHandle = TextureManager::Load("mapBack.png");
	playerHandle= TextureManager::Load("mapPlayer.png");
	//スプライト生成
	backSprite = Sprite::Create(backHandle, { 50,50 });
	playerSprite = Sprite::Create(playerHandle, { 203 + player->GetPlayerWorldTransform().translation_.x,153 - player->GetPlayerWorldTransform().translation_.z }, { 1,1,1,1 }, { 0.5f,0.5f });
}

void Map::Update()
{
	Vector2 playerPosition = playerSprite->GetPosition();
	playerPosition.x = 203 + player->GetPlayerWorldTransform().translation_.x;
	playerPosition.y = 153 - player->GetPlayerWorldTransform().translation_.z;
	playerSprite->SetPosition(playerPosition);
}

void Map::Draw()
{
	backSprite->Draw();
	playerSprite->Draw();
}