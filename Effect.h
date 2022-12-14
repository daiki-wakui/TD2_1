#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "DebugText.h"
#include <random>
#include "Player.h"

class Effect {
private:
	WorldTransform worldTransform_;
	WorldTransform worldTransformbig_;
	WorldTransform popWorldTransform_;

	Model* model_ = nullptr;
	Model* boxModel_ = nullptr;
	uint32_t textureHandle_ = 0;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	Player* player_ = nullptr;

	int isAnimation = false;
	Vector3 Power;
	Vector3 PowerBig;
	float fallPowerY = 0;
	int change = 0;
	int isDead = 0;
	int timer = 0;

public:
	~Effect();
	void Initialize(Player* player,WorldTransform worldTransform ,Model* model, uint32_t textureHandle, const int& Case);
	void Update();
	void Draw(ViewProjection& viewProjection);

	int IsDead() { return isDead; };
};
