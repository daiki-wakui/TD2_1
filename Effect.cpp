#include "Effect.h"
#include <cassert>

Effect::~Effect()
{
	delete boxModel_;
}

void Effect::Initialize(Player* player,WorldTransform worldTransform,Model* model, uint32_t textureHandle, const int& Case) {
	//NULLポインタチェック
	assert(model);

	//引数のデータをメンバ変数に代入
	player_ = player;
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_ = worldTransform;
	worldTransformbig_ = worldTransform;

	boxModel_ = Model::CreateFromOBJ("box", true);

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	popWorldTransform_.scale_ = { 1.5f,15.0f,1.5f };

	//ワールド座標変換の初期化
	worldTransform_.Initialize();
	worldTransformbig_.Initialize();
	popWorldTransform_.Initialize();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float> posYDist(0.5f, 1.0f);
	std::uniform_real_distribution<float> posXDist(-0.5f, 0.5f);

	fallPowerY = 0.1f;
	if (Case == 0) {
		worldTransform_.scale_ = { 0.5,0.5,0.5 };
		Power.y = posYDist(engine);
		Power.x = posXDist(engine);
		Power.z = posXDist(engine);
	}
	else if (Case == 1) {
		Power.x = 0.25f;
		Power.y = 0.25f;
		Power.z = 0.25f;

		worldTransform_.scale_ = { 3.0f,3.0f,3.0f };
	}
	else if (Case == 2) {
		worldTransformbig_.scale_ = { 0.15,0.15,0.15 };
		worldTransform_.scale_ = { 0.35,0.35,0.35 };
		Power.y = posYDist(engine) * (player_->GetAttackWorldTransform().scale_.x / 8);
		Power.x = posXDist(engine) * (player_->GetAttackWorldTransform().scale_.x / 4);
		Power.z = posXDist(engine) * (player_->GetAttackWorldTransform().scale_.z / 4);
		PowerBig.y = posYDist(engine) * (player_->GetAttackWorldTransform().scale_.x / 8);
		PowerBig.x = posXDist(engine)* (player_->GetAttackWorldTransform().scale_.x / 4);
		PowerBig.z = posXDist(engine)* (player_->GetAttackWorldTransform().scale_.z / 4);
	}

	change = Case;
}

void Effect::Update() {

	switch (change)
	{
	case 0:	//爆発
		timer++;

		fallPowerY -= 0.01f;
		Power.y += fallPowerY;
		worldTransform_.rotation_.x += 0.15f;
		worldTransform_.rotation_.y += 0.05f;
		if (Power.x <= 0) {
			Power.x -= 0.005f;
		}
		else {
			Power.x += 0.005f;
		}

		/*if (Power.y <= 0) {
			Power.y -= 0.1f;
		}
		else {
			Power.y += 0.1f;
		}*/

		worldTransform_.translation_.x += Power.x;
		worldTransform_.translation_.y += Power.y;
		worldTransform_.translation_.z += Power.z;

		if (timer > 50) {
			isDead = true;
			timer = 0;
		}

		break;

	case 1:	//発生
		/*if (worldTransform_.scale_.y > 0) {
			Power.y += 0.5f;
			worldTransform_.scale_.y -= Power.y;
		}*/

		if (worldTransform_.scale_.x > 0) {
			worldTransform_.rotation_.z += 0.25f;
			worldTransform_.rotation_.y += 0.25f;
			worldTransform_.rotation_.x += 0.25f;
			worldTransform_.scale_ -= Power;
		}
		else {
			isDead = true;
		}



		break;

	case 2:

		timer++;

		worldTransform_.rotation_.x += 0.35f;
		worldTransform_.rotation_.y += 0.35f;

		worldTransformbig_.rotation_.x += 0.55f;
		worldTransformbig_.rotation_.y += 0.55f;
		worldTransformbig_.rotation_.z += 0.55f;

		if (Power.x <= 0) {
			Power.x -= 0.005f;
		}
		else {
			Power.x += 0.005f;
		}

		worldTransform_.translation_.x += Power.x;
		worldTransform_.translation_.y += Power.y;
		worldTransform_.translation_.z += Power.z;

		worldTransformbig_.translation_ += PowerBig;

		if (timer > 50) {
			isDead = true;
			timer = 0;
		}

		break;
	}

	//行列変換
	MathUtility::MatrixCalculation(worldTransform_);
	MathUtility::MatrixCalculation(worldTransformbig_);
	MathUtility::MatrixCalculation(popWorldTransform_);
	//行列を送信
	worldTransform_.TransferMatrix();
	worldTransformbig_.TransferMatrix();
	popWorldTransform_.TransferMatrix();
}

void Effect::Draw(ViewProjection& viewProjection) {
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	if (change == 2) {
		model_->Draw(worldTransformbig_, viewProjection, textureHandle_);
	}
	//boxModel_->Draw(popWorldTransform_, viewProjection, textureHandle_);
}