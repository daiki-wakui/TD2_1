#pragma once
#include"Input.h"
#include "myMath.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"

class Player
{
private:

	//クラス読み込み
	Input* input = nullptr;

	Model* model = nullptr;

	uint32_t textureHundle = 0;
	//ワールドトランスフォーム
	WorldTransform worldTransform;
	//ビュープロジェクション
	ViewProjection viewProjection;
	
	float speed = 0.0f;

public:

	Player();
	~Player();
	void Initialize();
	void Update();
	void Draw();

	//ゲッター
	WorldTransform GetWorldTransform();
	float GetSpeed();
};