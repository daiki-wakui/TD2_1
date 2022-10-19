#pragma once
#include"Sprite.h"
#include"Player.h"
#include"Enemy.h"

class Map
{
private:

	Player* player = nullptr;

	//テクスチャハンドル
	uint32_t backHandle = 0;
	uint32_t playerHandle = 0;
	uint32_t enemyHandle = 0;

	Sprite* backSprite = nullptr;
	Sprite* playerSprite = nullptr;
	Sprite* enemySprite[100];

	int generate = 0;
	//Vector2 enemyPosition;

public:
	Map();
	~Map();
	void Initialize(ViewProjection viewProjection);
	void Update(std::list<std::unique_ptr<Enemy>>& enemys);
	void Draw(std::list<std::unique_ptr<Enemy>>& enemys);

	void SetGenerate(int gen);
};