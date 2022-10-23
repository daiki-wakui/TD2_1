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
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	ViewProjection viewProjection;
	ViewProjection oldViewProjection;

	Model* model = nullptr;
	uint32_t texture;

	Model* floorModel = nullptr;
	WorldTransform floorWorldTransform;

	Model* spawn_ = nullptr;
	/*Model* spawn2_ = nullptr;
	Model* spawn3_ = nullptr;
	Model* spawn4_ = nullptr;
	Model* spawn5_ = nullptr;*/

	WorldTransform worldtransform_;
	WorldTransform spawnEnemyCircle;
	/*WorldTransform worldtransform3_;
	WorldTransform worldtransform4_;
	WorldTransform worldtransform5_;*/

	myMath::Vector3 spawnRightTopPos;
	/*myMath::Vector3 spawnRightBottomPos;
	myMath::Vector3 spawnLeftTopPos;
	myMath::Vector3 spawnLeftBottomPos;*/
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
};
