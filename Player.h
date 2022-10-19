#pragma once
#include"Input.h"
#include "myMath.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
#include"MathUtility.h"
#include "DebugText.h"

class Player
{
private:

	//�N���X�ǂݍ���
	Input* input = nullptr;
	DebugText* debugText_ = nullptr;

	Model* playerModel = nullptr;
	Model* taleModel = nullptr;
	Model* attackModel = nullptr;

	uint32_t textureHundle = 0;
	//���[���h�g�����X�t�H�[��
	WorldTransform playerWorldTransform;//�v���C���[�̒��S���W
	WorldTransform taleWorldTransform;//�v���C���[�̂��̍��W
	WorldTransform attackWorldTransform;//�����̒��S���W

	myMath::Vector3 player;
	myMath::Vector3 front;
	myMath::Vector3 frontPosition;
	myMath::Vector3 frontVec;
	myMath::Vector3 normFrontVec;

	float speed = 0.0f;
	float bombCharge = 1.0f;
	float angle = 0.0f;
	bool attackFlag = false;
	int attackTimer = 10;

public:

	Player();
	~Player();
	void Initialize(ViewProjection viewProjection);
	void Update();
	void Move();
	void MoveLimit();
	void Rotation();
	void Attack();
	void Tale();
	void Draw(ViewProjection viewProjection);

	//�Q�b�^�[
	WorldTransform GetPlayerWorldTransform();//�v���C���[�̒��S���W
	WorldTransform GetTaleWorldTransform();//�v���C���[�̂��̍��W
	WorldTransform GetAttackWorldTransform();//�����̒��S���W
	float GetSpeed();
	float GetBombCharge();//�����̔��a
	static Player* GetInstance();
};