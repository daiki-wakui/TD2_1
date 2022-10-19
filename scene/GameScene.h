#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"Player.h"
#include"Map.h"

#include "Enemy.h"

#include <list>
#include <memory>

#include "sstream"
#include "fstream"

#include "Score.h"

enum SceneName
{
	Title,
	Game,
	Result,
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵発生
	/// </summary>
	void EnemyOcurrence();


  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	ViewProjection viewProjection;

	Model* model = nullptr;
	uint32_t texture;

	

	//敵リスト
	std::list<std::unique_ptr<Enemy>> enemys;

	int enemyGeneration = 0;

	bool isWait = false;

	int waitTimer = 300;

	Player* player = nullptr;
	Score* score = nullptr;

	std::unique_ptr<Map>map;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	int scene = 0;
};
