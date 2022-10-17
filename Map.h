#pragma once
#include"Sprite.h"
#include"Player.h"

class Map
{
private:
	//テクスチャハンドル
	uint32_t backHandle = 0;
	uint32_t playerHandle = 0;
	uint32_t enemyHandle = 0;

	Sprite* backSprite = nullptr;
	Sprite* playerSprite = nullptr;
	Player* player = nullptr;

public:
	Map();
	~Map();
	void Initialize(ViewProjection viewProjection);
	void Update();
	void Draw();
};