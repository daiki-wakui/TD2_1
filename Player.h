#pragma once
#include"Input.h"
#include"Mesh.h"
#include "myMath.h"

class Player
{
private:

	//クラス読み込み
	Input* input = nullptr;
	Mesh* mesh = nullptr;

	myMath::Vector3 player = { 0,0,0 };
	float speed = 0.0f;

public:

	void Initialize();
	void Update();
	void Draw();

	//ゲッター
	myMath::Vector3 GetPlayerPos();
	float GetSpeed();
	static Player* GetInstance();
};