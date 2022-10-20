#pragma once
#include"Sprite.h"
#include"Player.h"
#include"Enemy.h"
#include"EnemyStraight.h"

class Map
{
private:

	Player* player = nullptr;

	//テクスチャハンドル
	uint32_t backHandle = 0;
	uint32_t objectHandle = 0;
	uint32_t enemyHandle = 0;

	Sprite* backSprite = nullptr;
	Sprite* playerSprite = nullptr;
	Sprite* enemySprite[100];
	Sprite* enemyStraightSprite[100];

	//Vector2 enemyPosition;

public:
	Map();
	~Map();

	/// <summary>
	/// プレイヤーとマップの初期化処理
	/// </summary>
	void Initialize(ViewProjection viewProjection);

	
	void Update();

	/// <summary>
	/// 敵の更新処理
	/// </summary>
	/// <param name="enemys">敵のlist名</param>
	/// <param name="generate">生成クールタイム</param>
	void EnemyUpdate(std::list<std::unique_ptr<Enemy>>& enemys, int& generate);

	void EnemyStraightUpdate(std::list<std::unique_ptr<EnemyStraight>>& enemys, int& generate);

	/// <summary>
	/// プレイヤーと背景の描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵の描画処理
	/// <param name="enemys">敵のlist名</param>
	/// </summary>
	void EnemyDraw(std::list<std::unique_ptr<Enemy>>& enemys);

	void EnemyStraightDraw(std::list<std::unique_ptr<EnemyStraight>>& enemys);
};