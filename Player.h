#pragma once
#include"Input.h"
#include "myMath.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"

class Player
{
private:

	//�N���X�ǂݍ���
	Input* input = nullptr;

	Model* model = nullptr;

	uint32_t textureHundle = 0;
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection;
	
	float speed = 0.0f;

public:

	Player();
	~Player();
	void Initialize();
	void Update();
	void Draw();

	//�Q�b�^�[
	WorldTransform GetWorldTransform();
	float GetSpeed();
};