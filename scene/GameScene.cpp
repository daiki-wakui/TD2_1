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

		//デスフラグが立った敵を削除
		enemys.remove_if([](std::unique_ptr<Enemy>& enemy_) { return enemy_->GetIsDead(); });
		viewProjection.eye = { player->GetPlayerWorldTransform().translation_.x,75,player->GetPlayerWorldTransform().translation_.z-20 };
		viewProjection.target = { player->GetPlayerWorldTransform().translation_.x,0,player->GetPlayerWorldTransform().translation_.z };
		player->Update();

		score->Update();

		map->SetGenerate(enemyGeneration);
		EnemyOcurrence({100.0f,0.0f,0.0f});
		map->Update(enemys);
		for (const std::unique_ptr<Enemy>& enemy : enemys)
		{
			enemy->Update();
		}

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

void GameScene::EnemyOcurrence(const myMath::Vector3 p) {
	if (enemys.size() < 10)
	{
		if (enemyGeneration == 0)
		{
			myMath::Vector3 position = { p.x,p.y,p.z };
			//Enemyを生成し、初期化
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(model, position,texture);
			//Enemyを登録する
			enemys.push_back(std::move(newEnemy));

			enemyGeneration = 0;
		}
	}

	enemyGeneration++;
	if (enemyGeneration > 180)
	{
		enemyGeneration = 0;
	}
}
