#pragma once
#include "Score.h"
#include"myMath.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include "Model.h"
#include"Player.h"
class EnemyBomb
{
public:
	void Initialize(ViewProjection viewProjection, myMath::Vector3 position);
	void Update();
	void Move();
	void Draw(ViewProjection viewProjection);
	bool GetIsDead();
	void Limit();
	void Collider();

	WorldTransform GetWorldTransform();

private:
	WorldTransform worldTransform;
	ViewProjection viewProjection;
	Model* model = nullptr;
	uint32_t textureHandle = 0;
	Player* player = nullptr;
	Score* score = nullptr;
	
	myMath::Vector3 pos = { 0,0,0 };
	myMath::Vector3 moveVec;
	myMath::Vector3 normMoveVec;

	float speed = 5.0f;
	bool isDead = false;
	float radius = 2.0f;

};

