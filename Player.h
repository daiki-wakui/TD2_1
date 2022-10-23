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

	Model* playerModel_ = nullptr;
	Model* taleModel_ = nullptr;

	uint32_t textureHundle = 0;
	//���[���h�g�����X�t�H�[��
	WorldTransform playerWorldTransform;//�v���C���[�̒��S���W
	WorldTransform playerModelWorldTransform;//�v���C���[���f���̒��S���W
	WorldTransform taleWorldTransform;//�v���C���[�̂��̍��W
	WorldTransform taleModelWorldTransform;//�v���C���[�̂����f���̍��W
	WorldTransform attackWorldTransform;//�����̒��S���W

	myMath::Vector3 player;
	myMath::Vector3 front;
	myMath::Vector3 frontPosition;
	myMath::Vector3 frontVec;
	myMath::Vector3 normFrontVec;

	float speed = 0.0f;
	float bombCharge = 0.0f;
	float angle = 0.0f;
	bool attackFlag = false;
	int attackTimer = 0;

	int LimitPosX = 143;
	int LimitPosZ = 93;
	int LimitPosNX = -143;
	int LimitPosNZ = -93;

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
	void Reset();

	//�Q�b�^�[
	WorldTransform GetPlayerWorldTransform();//�v���C���[�̒��S���W
	WorldTransform GetTaleWorldTransform();//�v���C���[�̂��̍��W
	WorldTransform GetAttackWorldTransform();//�����̒��S���W
	float GetSpeed();
	float GetBombCharge();//�����̔��a
	bool GetAttackFlag();
	static Player* GetInstance();
};