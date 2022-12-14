#pragma once
#include"Input.h"
#include "myMath.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
#include"MathUtility.h"
#include "DebugText.h"
#include"AudioManager.h"

class Player
{
private:

	//クラス読み込み
	Input* input = nullptr;
	DebugText* debugText_ = nullptr;
	AudioManager* audioManager = nullptr;

	Model* playerModel = nullptr;
	Model* taleModel = nullptr;
	Model* attackModel = nullptr;

	Model* playerModel_ = nullptr;
	Model* taleModel_ = nullptr;

	uint32_t textureHundle = 0;
	//ワールドトランスフォーム
	WorldTransform playerWorldTransform;//プレイヤーの中心座標
	WorldTransform playerModelWorldTransform;//プレイヤーモデルの中心座標
	WorldTransform taleWorldTransform;//プレイヤーのけつの座標
	WorldTransform taleModelWorldTransform;//プレイヤーのけつモデルの座標
	WorldTransform attackWorldTransform;//爆発の中心座標

	myMath::Vector3 player;
	myMath::Vector3 front;
	myMath::Vector3 frontPosition;
	myMath::Vector3 frontVec;
	myMath::Vector3 normFrontVec;

	float speed = 0.0f;
	float bombCharge = 0.0f;
	float angle = 0.0f;
	bool attackFlag = false;
	int attackTimer = 0;

	int LimitPosX = 143;
	int LimitPosZ = 93;
	int LimitPosNX = -143;
	int LimitPosNZ = -93;

	bool isStart_ = false;
	int timer = 0;

	uint32_t bombSE;
	uint32_t bombChargeSE;
	uint32_t limitHitSE;

	bool chargeSEFlag = false;

public:

	Player();
	~Player();
	void Initialize(ViewProjection viewProjection);
	void Update(int& scene);
	void Move();
	void MoveLimit();
	void Rotation();
	void Attack();
	void Tale(int scene);
	void Draw(ViewProjection viewProjection);
	void Reset();
	void titleSceneUpdate(int& scene);
	void titleSceneRotation();

	//ゲッター
	WorldTransform GetPlayerWorldTransform();//プレイヤーの中心座標
	WorldTransform GetTaleWorldTransform();//プレイヤーのけつの座標
	WorldTransform GetAttackWorldTransform();//爆発の中心座標
	float GetSpeed();
	float GetBombCharge();//爆発の半径
	bool GetAttackFlag();
	static Player* GetInstance();
	int GetTimer() { return timer; };

	void SetChargeSEFlag(bool flag);
};