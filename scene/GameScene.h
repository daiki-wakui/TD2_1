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
#include <DirectXMath.h>

enum SceneName
{
	Title,
	Tutorial,
	Game,
	Result,
};

enum Spawner
{
	MiddleTop,
	MiddleCenter,
	MiddleBottom,
	LeftTop,
	LeftCenter,
	LeftBottom,
	RightTop,
	RightCenter,
	RightBottom,
	LMTop,
	LMBottom,
	RMTop,
	RMBottom,
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

	/*void EnemyBombsGen(const myMath::Vector3& p);
	void EnemyBombsSpawn(const myMath::Vector3& p);*/

	void Reset();

	void SpawnCollider();
	
	void SpawnEmerge();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	AudioManager* audioManager = nullptr;
	DebugText* debugText_ = nullptr;
	ViewProjection viewProjection;
	ViewProjection oldViewProjection;

	uint32_t mapSpawnerHandle = 0;
	Sprite* mapSpawnerSprite[13] = { nullptr };

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

	UINT nowTime = 0;
	UINT nowFlame = 0;

	//スポーン地点関連
	//各スポーン地点の座標
	WorldTransform spawnWorldTransform[13];

	myMath::Vector3 spawnMiddleTop = { 0.0f,0.0f,50.0f };		//中心上
	myMath::Vector3 spawnMiddleCenter = { 0,0,0 };				//中心
	myMath::Vector3 spawnMiddleBottom = { 0.0f,0.0f,-50.0f };	//中心下

	myMath::Vector3 spawnLeftTop = { -100.0f,0.0f,50.0f };		//左上
	myMath::Vector3 spawnLeftCenter = { -100.0f,0.0f,0.0f };	//左中心
	myMath::Vector3 spawnLeftBottom = { -100.0f,0.0f,-50.0f };	//左下	

	myMath::Vector3 spawnRightTop = { 100.0f,0.0f,50.0f };		//右上
	myMath::Vector3 spawnRightCenter = { 100.0f,0.0f,0.0f };		//右中心
	myMath::Vector3 spawnRightBottom = { 100.0f,0.0f,-50.0f };	//右下

	myMath::Vector3 spawnLMTop = { -50.0f,0.0f,25.0f };			//左と真ん中の間の上
	myMath::Vector3 spawnLMBottom = { -50.0f,0.0f,-25.0f };		//左と真ん中の間の下
	myMath::Vector3 spawnRMTop = { 50.0f,0.0f,25.0f };			//右と真ん中の間の上
	myMath::Vector3 spawnRMBottom = { 50.0f,0.0f,-25.0f };		//右と真ん中の間の下

	//各スポーン地点のフラグ
	bool isSpawn[13];

	int spawnNum;

	int spawnTimer = 0;

	int spawnCount = 0;

	bool startSpawn = true;

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

	uint32_t TutorialBombChargeSE;
	bool TutorialChargeSEFlag = false;

	float tutorialUIpower;
	int tutorialCount;
	int tutorialMoveChack;
	int isTutorial = true;
	int tutorialFinishTime;
	bool isTutorialFinish = false;
	uint32_t tutorialFinCountTex[3];
	Sprite* spritetutorialFin1 = nullptr;
	Sprite* spritetutorialFin2 = nullptr;
	Sprite* spritetutorialFin3 = nullptr;
	uint32_t tutorialTexture[2];
	Sprite* spriteTutorial1 = nullptr;
	Sprite* spriteTutorial2 = nullptr;
	uint32_t tutorialUITex_;
	Sprite* spriteTutorialUI_ = nullptr;
	uint32_t tutorialFinishTex_;
	Sprite* spriteTutorialFinish_ = nullptr;
	uint32_t tutorialSkipTex_;
	Sprite* spriteTutorialSkip = nullptr;



	int scene = 0;
	uint32_t titleSceneBGM = 0;
	uint32_t gameSceneBGM = 0;
	uint32_t tutorialSceneBGM = 0;
	uint32_t resultSceneBGM = 0;

	std::list<std::unique_ptr<Effect>> effects_;
	uint32_t redTexture_;
	uint32_t blueTexture_;
	uint32_t whiteTexture_;
	uint32_t orangeTexture_;
	uint32_t purpleTexture_;

	int isAnimation = 0;
	WorldTransform effectWorldTransform;
	WorldTransform World;

	Model* exModel_ = nullptr;
	WorldTransform explosionTransform;
	bool isExplosion = false;


	uint32_t titleUITex_ = 0;
	Sprite* spriteTitleUI_ = nullptr;
	uint32_t titleTextrue_ = 0;
	Sprite* spriteTielelogo_ = nullptr;
	myMath::Vector2 logoPos = { 0,0 };
	float logoRot;

	bool isStart = false;
	bool isControl = false;
	
	uint32_t scsneChangeTexture_;
	Sprite* spriteSceneChange = nullptr;
	myMath::Vector2 sceneChangePos = { 0,0 };

	uint32_t damageSE;
	uint32_t spawnerBreakSE;

	bool GameFinish = false;
	bool ChangeTitle = false;
	bool changetutorial = false;

	int resultTimer;

	uint32_t resultframeTex_;
	uint32_t scoreTex_;
	uint32_t rankTex_[5];
	Sprite* spriteResultframeTex_;
	Sprite* spriteScoreTex_;
	Sprite* spriteScoreRank_C;
	Sprite* spriteScoreRank_B;
	Sprite* spriteScoreRank_A;
	Sprite* spriteScoreRank_S;
	Sprite* spriteScoreRank_Splus;
};
