#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	model = Model::Create();
	viewProjection.eye = { 0,75,-10 };
	viewProjection.Initialize();
	oldViewProjection.eye = { 0,75,-10 };
	oldViewProjection.Initialize();

	player = Player::GetInstance();
	player->Initialize(viewProjection);

	texture = TextureManager::Load("mario.jpg");
	
	map = std::make_unique<Map>();
	map->Initialize(viewProjection);

	score = Score::GetInstance();
	score->Initialize();

	spawn_ = Model::Create();
	worldtransform_.scale_ = { 2.0f,2.0f,2.0f };
	worldtransform_.Initialize();
	
	spawn2_ = Model::Create();
	worldtransform2_.scale_ = { 2.0f,2.0f,2.0f };
	worldtransform2_.Initialize();

	spawn3_ = Model::Create();
	worldtransform3_.scale_ = { 2.0f,2.0f,2.0f };
	worldtransform3_.Initialize();

	spawn4_ = Model::Create();
	worldtransform4_.scale_ = { 2.0f,2.0f,2.0f };
	worldtransform4_.Initialize();

	spawn5_ = Model::Create();
	worldtransform5_.scale_ = { 2.0f,2.0f,2.0f };
	worldtransform5_.Initialize();

	spawnRightTopPos = { 125.0f,0.0f,75.0f };
	spawnLeftBottomPos = { -125.0f,0.0f,-75.0f };
	spawnCenterPos = { 0,0,0 };
	spawnRightBottomPos = { 125.0f,0.0f,-75.0f };
	spawnLeftTopPos = { -125.0f,0.0f,75.0f };
}

void GameScene::Update()
{
	//ゲームのシーン管理
	switch (scene)
	{
		//タイトル画面
	case Title:
		if (input_->TriggerKey(DIK_SPACE)) {
			scene++;	//ゲームシーンへ
		}

		break;

		//ゲームシーン
	case Game:

		worldtransform_.translation_.x = spawnRightTopPos.x;
		worldtransform_.translation_.z = spawnRightTopPos.z;

		worldtransform2_.translation_.x = spawnLeftBottomPos.x;
		worldtransform2_.translation_.z = spawnLeftBottomPos.z;

		worldtransform3_.translation_.x = spawnLeftTopPos.x;
		worldtransform3_.translation_.z = spawnLeftTopPos.z;

		worldtransform4_.translation_.x = spawnRightBottomPos.x;
		worldtransform4_.translation_.z = spawnRightBottomPos.z;

		worldtransform5_.translation_.x = spawnCenterPos.x;
		worldtransform5_.translation_.z = spawnCenterPos.z;


		//デスフラグが立った敵を削除
		enemys.remove_if([](std::unique_ptr<Enemy>& enemy_) { return enemy_->GetIsDead(); });
		viewProjection.eye = { player->GetPlayerWorldTransform().translation_.x,75,player->GetPlayerWorldTransform().translation_.z-20 };
		viewProjection.target = { player->GetPlayerWorldTransform().translation_.x,0,player->GetPlayerWorldTransform().translation_.z };
		player->Update();

		score->Update();

		map->SetGenerate(enemyGeneration);

		//生成処理
		EnemySpawn(spawnRightTopPos);	//右
		EnemySpawn(spawnLeftBottomPos);	//左
		EnemySpawn(spawnLeftTopPos);	//右
		EnemySpawn(spawnRightBottomPos);	//左
		EnemySpawn(spawnCenterPos);	//右

		//スポーンクールタイム
		enemyGeneration++;

		if (enemyGeneration > 50)
		{
			enemyGeneration = 0;
		}

		map->Update(enemys);
		for (const std::unique_ptr<Enemy>& enemy : enemys)
		{
			enemy->Update();
		}

		MathUtility::MatrixCalculation(worldtransform_);//行列の更新
		worldtransform_.TransferMatrix();
		MathUtility::MatrixCalculation(worldtransform2_);//行列の更新
		worldtransform2_.TransferMatrix();
		MathUtility::MatrixCalculation(worldtransform3_);//行列の更新
		worldtransform3_.TransferMatrix();
		MathUtility::MatrixCalculation(worldtransform4_);//行列の更新
		worldtransform4_.TransferMatrix();
		MathUtility::MatrixCalculation(worldtransform5_);//行列の更新
		worldtransform5_.TransferMatrix();

		viewProjection.TransferMatrix();
		viewProjection.UpdateMatrix();
		debugText_->SetPos(20, 40);
		debugText_->Printf("%d", enemys.size());
		break;


		//リザルト画面
	case Result:

		break;
	}
	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//ゲームのシーン管理
	switch (scene)
	{
		//タイトル画面
	case Title:

		debugText_->SetPos(550, 300);
		debugText_->Printf("Title Scene Space Start");

		break;


		//ゲームシーン
	case Game:

		player->Draw(viewProjection);
		for (const std::unique_ptr<Enemy>& enemy : enemys)
		{
			enemy->Draw(viewProjection);
		}

		score->Draw();

		spawn_->Draw(worldtransform_, viewProjection, texture);
		spawn2_->Draw(worldtransform2_, viewProjection, texture);
		spawn3_->Draw(worldtransform3_, viewProjection, texture);
		spawn4_->Draw(worldtransform4_, viewProjection, texture);
		spawn5_->Draw(worldtransform5_, viewProjection, texture);

		break;


		//リザルト画面
	case Result:

		break;
	}

	debugText_->SetPos(20, 20);
	debugText_->Printf("Scene %d", scene);

	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	
	switch (scene)
	{
	case Game:

		map->Draw(enemys);

		break;
	}

	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//生成関数
void GameScene::EnemyOcurrence(const myMath::Vector3 p) {

	myMath::Vector3 position = { p.x,p.y,p.z };
	//Enemyを生成し、初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(model, position, texture);
	//Enemyを登録する
	enemys.push_back(std::move(newEnemy));
}

//生成関数を呼ぶ為のもの
void GameScene::EnemySpawn(const myMath::Vector3 p)
{
	if (enemys.size() < 100)
	{
		if (enemyGeneration == 0)
		{
			EnemyOcurrence(p);
		}
	}
}
