#pragma once

#include "AudioManager.h"
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
#include"MathUtility.h"
#include "myMath.h"

#include "Enemy.h"
#include"EnemyStraight.h"
#include "EnemyCircle.h"
#include "EnemyBomb.h"

#include <list>
#include <memory>

#include "sstream"
#include "fstream"

#include "Score.h"
#include "Effect.h"

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
	void EnemyOcurrence(const myMath::Vector3& p);
	void EnemySpawn(const myMath::Vector3& p);
	void EnemyStraightsGen(const myMath::Vector3& p, float angle);
	void EnemyStraightsSpawn(const myMath::Vector3& p, float angle);
	void EnemyCirclesGen(const myMath::Vector3& p, float angle);
	void EnemyCirclesSpawn(const myMath::Vector3& p, float angle);

	void EnemyBombsGen(const myMath::Vector3& p);
	void EnemyBombsSpawn(const myMath::Vector3& p);

	void Reset();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	AudioManager* audioManager = nullptr;
	DebugText* debugText_ = nullptr;
	ViewProjection viewProjection;
	ViewProjection oldViewProjection;

	Model* model = nullptr;
	Model* bombModel = nullptr;
	uint32_t texture;
	Model* boxModel = nullptr;

	Model* floorModel = nullptr;
	WorldTransform floorWorldTransform;

	Model* wallModel = nullptr;
	WorldTransform wallWorldTransform[4];

	Model* spawn_ = nullptr;

	WorldTransform worldtransform_;
	WorldTransform spawnEnemyCircle;

	myMath::Vector3 spawnRightTopPos;
	myMath::Vector3 spawnCenterPos;

	//敵リスト
	std::list<std::unique_ptr<Enemy>> enemys;
	std::list<std::unique_ptr<EnemyStraight>> enemyStraights;
	std::list <std::unique_ptr<EnemyCircle>> enemyCircles;
	std::list <std::unique_ptr<EnemyBomb>> enemyBombs;

	int enemyGeneration = 0;
	int enemyStraightsGen = 0;
	float enemyStraightAngle = 0;

	int enemyCirclesGen = 0;
	float enemyCircleAngle = 0;

	int enemyBombsGen = 0;

	bool isWait = false;

	int waitTimer = 300;

	Player* player = nullptr;
	Score* score = nullptr;

	std::unique_ptr<Map>map;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	int scene = 0;
	uint32_t titleScene = 0;
	uint32_t gameScene = 0;

	std::list<std::unique_ptr<Effect>> objs_;
	uint32_t redTexture_;
	uint32_t whiteTexture_;
	uint32_t orangeTexture_;
	bool isAnimation = false;
	WorldTransform effectWorldTransform;
	WorldTransform World;

	Model* exModel_ = nullptr;
	WorldTransform explosionTransform;
};
