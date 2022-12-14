#pragma once
#include "Score.h"
#include"myMath.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include "Model.h"
#include"Player.h"
#include"AudioManager.h"

class EnemyCircle
{
private:
	Player* player = nullptr;
	AudioManager* audioManager = nullptr;

	myMath::Vector3 pos;//?G?̍??W
	myMath::Vector3 front;
	myMath::Vector3 moveVec;
	myMath::Vector3 normMoveVec;

	float angle = 0.0f;
	float speed = 5.0f;
	float radius = 2.0f;
	float radian = 0.0f;
	bool isDead = false;
	bool wallHitIsDead = false;
	bool playerHit = false;


	uint32_t textureHundle = 0;
	WorldTransform worldTransform;
	Model* model = nullptr;
	Score* score = nullptr;

	Model* model_ = nullptr;

public:
	void Initialize(Model*model,ViewProjection viewProjection, myMath::Vector3 position, float enemyAngle);
	void Update(uint32_t& damageSE);
	void Move();
	void Draw(ViewProjection viewProjection);
	bool GetIsDead();
	bool GetWallHitIsDead();
	bool GetPlayerHit();
	void Limit();
	void Collider(uint32_t damageSE);

	WorldTransform GetWorldTransform();
};

