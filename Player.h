#pragma once
#include"Input.h"
#include "myMath.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
#include"MathUtility.h"
#include "DebugText.h"

class Player
{
private:

	//クラス読み込み
	Input* input = nullptr;
	DebugText* debugText_ = nullptr;

	Model* playerModel = nullptr;
	Model* taleModel = nullptr;
	Model* attackModel = nullptr;

	uint32_t textureHundle = 0;
	//ワールドトランスフォーム
	WorldTransform playerWorldTransform;//プレイヤーの中心座標
	WorldTransform taleWorldTransform;//プレイヤーのけつの座標
	WorldTransform attackWorldTransform;//爆発の中心座標

	myMath::Vector3 player;
	myMath::Vector3 front;
	myMath::Vector3 frontPosition;
	myMath::Vector3 frontVec;
	myMath::Vector3 normFrontVec;

	float speed = 0.0f;
	float bombCharge = 1.0f;
	float angle = 0.0f;
	bool attackFlag = false;
	int attackTimer = 10;

public:

	Player();
	~Player();
	void Initialize(ViewProjection viewProjection);
	void Update();
	void Move();
	void MoveLimit();
	void Rotation();
	void Attack();
	void Tale();
	void Draw(ViewProjection viewProjection);

	//ゲッター
	WorldTransform GetPlayerWorldTransform();//プレイヤーの中心座標
	WorldTransform GetTaleWorldTransform();//プレイヤーのけつの座標
	WorldTransform GetAttackWorldTransform();//爆発の中心座標
	float GetSpeed();
	float GetBombCharge();//爆発の半径
	static Player* GetInstance();
};