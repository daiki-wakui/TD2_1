#pragma once
#include "Enemy.h"
#include "myMath.h"
#include "WorldTransform.h"
class Enemy;
class Collider
{
public:


	void CtoCCollision(const myMath::Vector3 v1, WorldTransform worldTransform, const float radius);
	
private:


	Enemy* enemy = nullptr;
	

};

