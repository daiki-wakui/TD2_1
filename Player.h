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

	Model* playerModel = nullptr;
	Model* taleModel = nullptr;

	uint32_t textureHundle = 0;
	//���[���h�g�����X�t�H�[��
	WorldTransform playerWorldTransform;
	WorldTransform taleWorldTransform;

	myMath::Vector3 player;
	myMath::Vector3 front;
	myMath::Vector3 frontPosition;
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
	void Tale();
	void Draw(ViewProjection viewProjection);

	//�Q�b�^�[
	WorldTransform GetWorldTransform();
	float GetSpeed();
	static Player* GetInstance();
};