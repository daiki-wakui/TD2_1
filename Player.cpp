#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	delete playerModel;
}

void Player::Initialize(ViewProjection viewProjection)
{
	//シングルトン代入
	input = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audioManager = AudioManager::GetInstance();

	textureHundle = TextureManager::Load("sample.png");
	playerModel = Model::Create();
	playerWorldTransform.scale_ = { 3,3,3 };
	playerWorldTransform.Initialize();

	taleModel = Model::Create();
	taleWorldTransform.scale_;
	taleWorldTransform.Initialize();

	attackModel = Model::Create();
	attackWorldTransform.Initialize();

	playerModel_ = Model::CreateFromOBJ("player_body", true);
	taleModel_ = Model::CreateFromOBJ("player_tail", true);
	playerModelWorldTransform.scale_ = { 5,5,5 };
	playerModelWorldTransform.Initialize();
	taleModelWorldTransform.Initialize();


	player = { 0,0,-50 };
	front = { 0,0,-49 };
	angle = MathUtility::PI;
	bombCharge = 2.0f;
	attackTimer = 3;
	viewProjection.Initialize();

	bombSE = AudioManager::GetInstance()->LoadAudio("Resources/sound_SFX_explosion.mp3");//爆発SE読み込み
	bombChargeSE= AudioManager::GetInstance()->LoadAudio("Resources/235742_copyc4t_tf-sci-fi-sweep-2.mp3");//爆発チャージSE読み込み
	limitHitSE= AudioManager::GetInstance()->LoadAudio("Resources/sound_SFX_bounce.mp3");//壁に当たった時のSE読み込み
}

void Player::titleSceneUpdate(int& scene) {
	Tale(scene);//尻尾の処理
	titleSceneRotation();
	if (attackFlag == true)
	{
		Attack();//攻撃処理
	}

	speed -= 0.005f;

	if (speed <= 0.0f)
	{
		speed = 0.0f;
	}

	player.x += normFrontVec.x * speed;
	player.z += normFrontVec.z * speed;
	playerWorldTransform.translation_.x = player.x;
	playerWorldTransform.translation_.z = player.z;

	playerModelWorldTransform.rotation_ = playerWorldTransform.rotation_;
	playerModelWorldTransform.translation_ = playerWorldTransform.translation_;

	taleModelWorldTransform.scale_ = taleWorldTransform.scale_;
	taleModelWorldTransform.rotation_ = taleWorldTransform.rotation_;
	taleModelWorldTransform.rotation_.y += 1.57f;
	taleModelWorldTransform.translation_ = taleWorldTransform.translation_;

	MathUtility::MatrixCalculation(playerWorldTransform);//行列の更新
	playerWorldTransform.TransferMatrix();//行列の転送

	MathUtility::MatrixCalculation(playerModelWorldTransform);//行列の更新
	playerModelWorldTransform.TransferMatrix();//行列の転送

	MathUtility::MatrixCalculation(taleWorldTransform);//行列の更新
	taleWorldTransform.TransferMatrix();//行列の転送

	MathUtility::MatrixCalculation(taleModelWorldTransform);//行列の更新
	taleModelWorldTransform.TransferMatrix();//行列の転送

	MathUtility::MatrixCalculation(attackWorldTransform);//行列の更新
	attackWorldTransform.TransferMatrix();//行列の転送
}

void Player::Update(int& scene)
{
	Tale(scene);//尻尾の処理
	Rotation();//プレイヤーの回転処理
	Move();//プレイヤーの移動処理
	if (attackFlag == true)
	{
		Attack();//攻撃処理
	}

	playerModelWorldTransform.rotation_ = playerWorldTransform.rotation_;
	playerModelWorldTransform.translation_ = playerWorldTransform.translation_;

	taleModelWorldTransform.scale_ = taleWorldTransform.scale_;
	taleModelWorldTransform.rotation_ = taleWorldTransform.rotation_;
	taleModelWorldTransform.rotation_.y += 1.57f;
	taleModelWorldTransform.translation_ = taleWorldTransform.translation_;

	MathUtility::MatrixCalculation(playerWorldTransform);//行列の更新
	playerWorldTransform.TransferMatrix();//行列の転送

	MathUtility::MatrixCalculation(playerModelWorldTransform);//行列の更新
	playerModelWorldTransform.TransferMatrix();//行列の転送

	MathUtility::MatrixCalculation(taleWorldTransform);//行列の更新
	taleWorldTransform.TransferMatrix();//行列の転送

	MathUtility::MatrixCalculation(taleModelWorldTransform);//行列の更新
	taleModelWorldTransform.TransferMatrix();//行列の転送

	MathUtility::MatrixCalculation(attackWorldTransform);//行列の更新
	attackWorldTransform.TransferMatrix();//行列の転送
}

void Player::Move()
{

	MoveLimit();

	speed -= 0.005f;
	if (input->TriggerKey(DIK_SPACE))
	{
		frontVec = front - player;
		normFrontVec = frontVec.normalization();
		speed = 0.1f * (bombCharge/2);
		attackFlag = true;

		audioManager->PlayWave(bombSE, false);//爆発SEを再生
	}
	if (speed <= 0.0f)
	{
		speed = 0.0f;
	}

	player.x -= normFrontVec.x * speed;
	player.z -= normFrontVec.z * speed;
	playerWorldTransform.translation_.x = player.x;
	playerWorldTransform.translation_.z = player.z;
}

void Player::MoveLimit()
{
	if (player.x < -144)
	{
		if (speed < 0.2f) {
			player.x = -143.9f;
			//audioManager->GetInstance()->PlayWave(limitHitSE, false);//壁に触れた時の音を再生
		}
		else {
			normFrontVec.x = -normFrontVec.x;
			audioManager->GetInstance()->PlayWave(limitHitSE, false);//壁に触れた時の音を再生
		}
	}
	else if (player.x > 144)
	{
		if (speed < 0.2f) {
			player.x = 143.9f;
			//audioManager->GetInstance()->PlayWave(limitHitSE, false);//壁に触れた時の音を再生
		}
		else {
			normFrontVec.x = -normFrontVec.x;
			audioManager->GetInstance()->PlayWave(limitHitSE, false);//壁に触れた時の音を再生
		}
	}
	if (player.z < -94)
	{
		if (speed < 0.2f) {
			player.z = -93.9f;
			//audioManager->GetInstance()->PlayWave(limitHitSE, false);//壁に触れた時の音を再生
		}
		else {
			normFrontVec.z = -normFrontVec.z;
			audioManager->GetInstance()->PlayWave(limitHitSE, false);//壁に触れた時の音を再生
		}
	}
	else if (player.z > 94)
	{
		if (speed < 0.2f) {
			player.z = 93.9f;
			//audioManager->GetInstance()->PlayWave(limitHitSE, false);//壁に触れた時の音を再生
		}
		else {
			normFrontVec.z = -normFrontVec.z;
			audioManager->GetInstance()->PlayWave(limitHitSE, false);//壁に触れた時の音を再生
		}
	}
}

void Player::titleSceneRotation()
{
	const float rotSpeed = 0.1f;

	if (isStart_ == true)
	{
		if (angle != 0 && timer < 33)
		{
			timer++;
			angle += rotSpeed;
		}
		if (timer == 32)
		{
			frontVec = front - player;
			normFrontVec = frontVec.normalization();
			speed = 0.1f * 10.0f;
			attackFlag = true;
			timer++;
		}
	}

	if (input->TriggerKey(DIK_SPACE) && isStart_ == false)
	{
		isStart_ = true;
		front.x = player.x + sinf(angle) * 2;
		front.z = player.z + cosf(angle) * 2;
	}

	taleWorldTransform.translation_.x = player.x + sinf(angle) * 7;
	taleWorldTransform.translation_.z = player.z + cosf(angle) * 7;

	attackWorldTransform.translation_.x = player.x + sinf(angle) * 8;
	attackWorldTransform.translation_.z = player.z + cosf(angle) * 8;

	if (angle > 2 * MathUtility::PI)
	{
		angle = 0;//オーバーフロー回避処理
	}
	else if (angle < 0)
	{
		angle = 2 * MathUtility::PI;//オーバーフロー回避処理
	}

	playerWorldTransform.rotation_.y = angle;
	taleWorldTransform.rotation_.y = angle;
	attackWorldTransform.rotation_.y = angle;
}

void Player::Rotation()
{
	const float rotSpeed = 0.075f;

	if (input->TriggerKey(DIK_SPACE))
	{
		front.x = player.x + sinf(angle) * 2;
		front.z = player.z + cosf(angle) * 2;
	}
	taleWorldTransform.translation_.x = player.x + sinf(angle) * 7;
	taleWorldTransform.translation_.z = player.z + cosf(angle) * 7;

	attackWorldTransform.translation_.x = player.x + sinf(angle) * 8;
	attackWorldTransform.translation_.z = player.z + cosf(angle) * 8;

	if (input->PushKey(DIK_A))
	{
		angle -= rotSpeed;
	}

	if (input->PushKey(DIK_D))
	{
		angle += rotSpeed;
	}

	if (angle > 2 * MathUtility::PI)
	{
		angle = 0;//オーバーフロー回避処理
	}
	else if (angle < 0)
	{
		angle = 2 * MathUtility::PI;//オーバーフロー回避処理
	}

	playerWorldTransform.rotation_.y = angle;
	taleWorldTransform.rotation_.y = angle;
	attackWorldTransform.rotation_.y = angle;
}

void Player::Attack()
{
	attackTimer--;
	if (attackTimer <= 0)
	{
		bombCharge = 2.0f;
		attackTimer = 3;
		attackFlag = false;
	}
}

void Player::Tale(int scene)
{
	if (scene == 2)
	{
		if (input->TriggerReleseKey(DIK_SPACE))
		{
			chargeSEFlag = true;
		}
	}

	if (chargeSEFlag == true)
	{
		audioManager->StopWave(bombChargeSE);
		audioManager->PlayWave(bombChargeSE, true);//爆発チャージの音を再生
		chargeSEFlag = false;
	}
	audioManager->ChangeVolume(bombChargeSE, bombCharge / 20.0f);

	if (!input->PushKey(DIK_SPACE))
	{
		bombCharge += 0.375f;//離している間爆弾をチャージ
	}

	if (bombCharge > 20.0f)
	{
		bombCharge = 20.0f;//上限を設定
	}

	taleWorldTransform.scale_.x = 1.0 + bombCharge / 1.5f;//尻尾の長さ
	taleWorldTransform.scale_.y = 1.0 + bombCharge / 1.5f;//尻尾の長さ
	taleWorldTransform.scale_.z = 1.0 + bombCharge / 1.5f;//尻尾の長さ
	attackWorldTransform.scale_.x = 1.0 * bombCharge;
	attackWorldTransform.scale_.y = 1.0 * bombCharge;
	attackWorldTransform.scale_.z = 1.0 * bombCharge;
}

void Player::Draw(ViewProjection viewProjection)
{
	playerModel_->Draw(playerModelWorldTransform, viewProjection);
	taleModel_->Draw(taleModelWorldTransform, viewProjection);
}

void Player::Reset()
{
	speed = 0.0f;
	player = { 0,0,-50 };
	front = { 0,0,-49 };
	angle = MathUtility::PI;
	bombCharge = 2.0f;
	attackTimer = 3;
	attackFlag = false;
	timer = 0;
	isStart_ = false;
	chargeSEFlag = false;

	audioManager->StopWave(bombSE);
	audioManager->StopWave(bombChargeSE);
	audioManager->StopWave(limitHitSE);
}

WorldTransform Player::GetPlayerWorldTransform()
{
	return playerWorldTransform;
}

WorldTransform Player::GetTaleWorldTransform()
{
	return taleWorldTransform;
}

WorldTransform Player::GetAttackWorldTransform()
{
	return attackWorldTransform;
}

float Player::GetSpeed()
{
	return speed;
}

float Player::GetBombCharge()
{
	return bombCharge;
}

bool Player::GetAttackFlag()
{
	return attackFlag;
}

Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}

void Player::SetChargeSEFlag(bool flag)
{
	chargeSEFlag = flag;
}