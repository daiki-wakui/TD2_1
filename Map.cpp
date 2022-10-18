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
	enemyHandle = TextureManager::Load("mapPlayer.png");

	//スプライト生成
	backSprite = Sprite::Create(backHandle, { 50,50 });
	playerSprite = Sprite::Create(playerHandle, { 203 + player->GetPlayerWorldTransform().translation_.x,153 - player->GetPlayerWorldTransform().translation_.z }, { 1,1,1,1 }, { 0.5f,0.5f });
}

void Map::Update(std::list<std::unique_ptr<Enemy>>& enemys)
{
	Vector2 playerPosition = playerSprite->GetPosition();
	playerPosition.x = 203 + player->GetPlayerWorldTransform().translation_.x;
	playerPosition.y = 153 - player->GetPlayerWorldTransform().translation_.z;
	playerSprite->SetPosition(playerPosition);

	if (enemys.size() < 10)
	{
		if (generate == 0)
		{
			for (const std::unique_ptr<Enemy>& enemy : enemys)
			{
				enemySprite = Sprite::Create(enemyHandle, { 203 + enemy->GetWorldTransform().translation_.x,153 - enemy->GetWorldTransform().translation_.z }, { 1,0,0,1 }, { 0.5f,0.5f });
			}
		}
	}

	for (const std::unique_ptr<Enemy>& enemy : enemys)
	{
		Vector2 enemyPosition = enemySprite->GetPosition();
		enemyPosition.x = 203 + enemy->GetWorldTransform().translation_.x;
		enemyPosition.y = 153 - enemy->GetWorldTransform().translation_.y;
		enemySprite->SetPosition(enemyPosition);
	}
}

void Map::Draw(std::list<std::unique_ptr<Enemy>>& enemys)
{
	backSprite->Draw();
	playerSprite->Draw();
	for (const std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemySprite->Draw();
	}
}

void Map::SetGenerate(int gen)
{
	generate = gen;
}