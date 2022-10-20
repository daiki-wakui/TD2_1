#pragma once
#include"Sprite.h"
#include"Player.h"
#include"Enemy.h"
#include"EnemyStraight.h"

class Map
{
private:

	Player* player = nullptr;

	//�e�N�X�`���n���h��
	uint32_t backHandle = 0;
	uint32_t objectHandle = 0;
	uint32_t enemyHandle = 0;

	Sprite* backSprite = nullptr;
	Sprite* playerSprite = nullptr;
	Sprite* enemySprite[100];
	Sprite* enemyStraightSprite[100];

	//Vector2 enemyPosition;

public:
	Map();
	~Map();

	/// <summary>
	/// �v���C���[�ƃ}�b�v�̏���������
	/// </summary>
	void Initialize(ViewProjection viewProjection);

	
	void Update();

	/// <summary>
	/// �G�̍X�V����
	/// </summary>
	/// <param name="enemys">�G��list��</param>
	/// <param name="generate">�����N�[���^�C��</param>
	void EnemyUpdate(std::list<std::unique_ptr<Enemy>>& enemys, int& generate);

	void EnemyStraightUpdate(std::list<std::unique_ptr<EnemyStraight>>& enemys, int& generate);

	/// <summary>
	/// �v���C���[�Ɣw�i�̕`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �G�̕`�揈��
	/// <param name="enemys">�G��list��</param>
	/// </summary>
	void EnemyDraw(std::list<std::unique_ptr<Enemy>>& enemys);

	void EnemyStraightDraw(std::list<std::unique_ptr<EnemyStraight>>& enemys);
};