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
	objectHandle= TextureManager::Load("mapPlayer.png");

	//スプライト生成
	backSprite = Sprite::Create(backHandle, { 50,50 });
	playerSprite = Sprite::Create(objectHandle, { 203 + player->GetPlayerWorldTransform().translation_.x,153 - player->GetPlayerWorldTransform().translation_.z }, { 1,1,1,1 }, { 0.5f,0.5f });
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

void Map::EnemyUpdate(std::list<std::unique_ptr<Enemy>>& enemys, int& generate)
{
	if (generate == 0)
	{
		int count = 0;
		for (const std::unique_ptr<Enemy>& enemy : enemys)
		{
			enemySprite[count] = Sprite::Create(objectHandle, { 203 + enemy->GetWorldTransform().translation_.x,153 - enemy->GetWorldTransform().translation_.z }, { 1,0,0,1 }, { 0.5f,0.5f });
			count++;
		}
	}

	int count = 0;
	for (const std::unique_ptr<Enemy>& enemy : enemys)
	{
		Vector2 enemyPosition = enemySprite[count]->GetPosition();
		enemyPosition.x = 203 + enemy->GetWorldTransform().translation_.x;
		enemyPosition.y = 153 - enemy->GetWorldTransform().translation_.z;
		enemySprite[count]->SetPosition(enemyPosition);
		count++;
	}
}

void Map::EnemyDraw(std::list<std::unique_ptr<Enemy>>& enemys)
{
	int count = 0;
	for (const std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemySprite[count]->Draw();
		count++;
	}
}

void Map::EnemyStraightUpdate(std::list<std::unique_ptr<EnemyStraight>>& enemys, int& generate)
{
	if (generate == 0)
	{
		int count = 0;
		for (const std::unique_ptr<EnemyStraight>& enemy : enemys)
		{
			enemyStraightSprite[count] = Sprite::Create(objectHandle, { 203 + enemy->GetWorldTransform().translation_.x,153 - enemy->GetWorldTransform().translation_.z }, { 1,0,0,1 }, { 0.5f,0.5f });
			count++;
		}
	}

	int count = 0;
	for (const std::unique_ptr<EnemyStraight>& enemy : enemys)
	{
		Vector2 enemyPosition = enemyStraightSprite[count]->GetPosition();
		enemyPosition.x = 203 + enemy->GetWorldTransform().translation_.x;
		enemyPosition.y = 153 - enemy->GetWorldTransform().translation_.z;
		enemyStraightSprite[count]->SetPosition(enemyPosition);
		count++;
	}
}

void Map::EnemyStraightDraw(std::list<std::unique_ptr<EnemyStraight>>& enemys)
{
	int count = 0;
	for (const std::unique_ptr<EnemyStraight>& enemy : enemys)
	{
		enemyStraightSprite[count]->Draw();
		count++;
	}
}

void Map::EnemyCircleUpdate(std::list<std::unique_ptr<EnemyCircle>>& enemys, int& generate)
{
	if (generate == 0)
	{
		int count = 0;
		for (const std::unique_ptr<EnemyCircle>& enemy : enemys)
		{
			enemyCircleSprite[count] = Sprite::Create(objectHandle, { 203 + enemy->GetWorldTransform().translation_.x,153 - enemy->GetWorldTransform().translation_.z }, { 1,0,0,1 }, { 0.5f,0.5f });
			count++;
		}
	}

	int count = 0;
	for (const std::unique_ptr<EnemyCircle>& enemy : enemys)
	{
		Vector2 enemyPosition = enemyCircleSprite[count]->GetPosition();
		enemyPosition.x = 203 + enemy->GetWorldTransform().translation_.x;
		enemyPosition.y = 153 - enemy->GetWorldTransform().translation_.z;
		enemyCircleSprite[count]->SetPosition(enemyPosition);
		count++;
	}
}

void Map::EnemyCircleDraw(std::list<std::unique_ptr<EnemyCircle>>& enemys)
{
	int count = 0;
	for (const std::unique_ptr<EnemyCircle>& enemy : enemys)
	{
		enemyCircleSprite[count]->Draw();
		count++;
	}
}