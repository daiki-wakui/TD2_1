#pragma once
#include"Input.h"
#include "myMath.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
#include"MathUtility.h"

class Player
{
private:

	//�N���X�ǂݍ���
	Input* input = nullptr;

	Model* model = nullptr;

	uint32_t textureHundle = 0;
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform;
	myMath::Vector3 player;
	myMath::Vector3 front;
	myMath::Vector3 frontVec;
	myMath::Vector3 normFrontVec;

	float speed = 0.0f;
	float bombCharge = 1.0f;
	float angle = 0.0f;

public:

	Player();
	~Player();
	void Initialize(ViewProjection viewProjection);
	void Update();
	void Move();
	void Rotation();
	void Bomb();
	void Draw(ViewProjection viewProjection);

	//�Q�b�^�[
	WorldTransform GetWorldTransform();
	float GetSpeed();
	static Player* GetInstance();
};