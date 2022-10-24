#pragma once
#include "Score.h"
#include"myMath.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include "Model.h"
#include"Player.h"
class EnemyStraight
{
private:
	Player* player = nullptr;

	myMath::Vector3 pos;//ìGÇÃç¿ïW
	myMath::Vector3 front;
	myMath::Vector3 moveVec;
	myMath::Vector3 normMoveVec;

	float angle=0.0f;
	float speed = 5.0f;
	bool isDead = false;

	uint32_t textureHundle = 0;
	WorldTransform worldTransform;
	Model* model = nullptr;
	Score* score = nullptr;

	Model* model_ = nullptr;

	float radius = 2.0f;
public:
	void Initialize(Model* model,ViewProjection viewProjection,myMath::Vector3 position, float enemyAngle);
	void Update();
	void Move();
	void Draw(ViewProjection viewProjection);
	bool GetIsDead();
	void Limit();
	void Collider();

	WorldTransform GetWorldTransform();
};