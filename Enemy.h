#pragma once
#include <cassert>
#include "myMath.h"
#include"Mesh.h"
#include "Model.h"
#include"Player.h"
#include"Score.h"
#include "WorldTransform.h"

class Enemy
{
private:

	myMath::Vector3 enemy = { 0,0,0 };
	WorldTransform worldTransform;
	Mesh* mesh = nullptr;
	Model* model_ = nullptr;
	Player* player = nullptr;
	Score* score = nullptr;

	myMath::Vector3 enemyVec = { 0,0,0 };
	myMath::Vector3 normEnemyVec = { 0,0,0 };

	bool isDead = false;
	float radius = 2.0f;

	uint32_t texture_;

public:

	void Initialize(Model* model, const myMath::Vector3 position, uint32_t texture);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool GetIsDead(){return isDead;}

	void Move();
	WorldTransform GetWorldTransform();

	//void Score();
};
