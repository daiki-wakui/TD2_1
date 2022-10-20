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

#pragma region スポーン地点の生成

	spawn_ = Model::Create();
	worldtransform_.scale_ = { 2.0f,2.0f,2.0f };
	worldtransform_.Initialize();

	/*spawnEnemyCircle.scale_ = { 2.0f,2.0f,2.0f };
	spawnEnemyCircle.Initialize();*/

#pragma endregion
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

#pragma region worldTransformにスポーン地点の座標代入

		spawnRightTopPos = { 125.0f,0.0f,75.0f };

		worldtransform_.translation_.x = spawnRightTopPos.x;
		worldtransform_.translation_.z = spawnRightTopPos.z;

		spawnCenterPos = { 0,0,0 };
		spawnEnemyCircle.translation_.x = spawnCenterPos.x;
		spawnEnemyCircle.translation_.z = spawnCenterPos.z;

#pragma endregion

#pragma region 敵の削除処理
		//デスフラグが立った敵を削除
		enemys.remove_if([](std::unique_ptr<Enemy>& enemy_) { return enemy_->GetIsDead(); });
		enemyStraights.remove_if([](std::unique_ptr<EnemyStraight>& enemy_) { return enemy_->GetIsDead(); });
		enemyCircles.remove_if([](std::unique_ptr<EnemyCircle>& enemy_) { return enemy_->GetIsDead(); });

#pragma endregion

		viewProjection.eye = { player->GetPlayerWorldTransform().translation_.x,75,player->GetPlayerWorldTransform().translation_.z-20 };
		viewProjection.target = { player->GetPlayerWorldTransform().translation_.x,0,player->GetPlayerWorldTransform().translation_.z };
		player->Update();

		score->Update();

#pragma region 敵の生成処理

		EnemySpawn(spawnRightTopPos);	//右

		enemyStraightAngle += 0.02f;
		if (enemyStraightAngle > 2 * MathUtility::PI)
		{
			enemyStraightAngle = 0;//オーバーフロー回避処理
		}
		else if (enemyStraightAngle < 0)
		{
			enemyStraightAngle = 2 * MathUtility::PI;//オーバーフロー回避処理
		}
		EnemyStraightsSpawn({ 0,0,0 }, enemyStraightAngle);


		enemyCircleAngle -= 0.2f;
		if (enemyCircleAngle > 2 * MathUtility::PI)
		{
			enemyCircleAngle = 0;//オーバーフロー回避処理
		}
		else if (enemyCircleAngle < 0)
		{
			enemyCircleAngle = 2 * MathUtility::PI;//オーバーフロー回避処理
		}
		EnemyCirclesSpawn({ 20,0,20 }, enemyCircleAngle);

#pragma endregion

#pragma region マップ関連

		map->Update();
		map->EnemyUpdate(enemys, enemyGeneration);
		map->EnemyStraightUpdate(enemyStraights,enemyStraightsGen);
		map->EnemyCircleUpdate(enemyCircles,enemyCirclesGen);

#pragma endregion

#pragma region クールタイム処理
		//スポーンクールタイム
		enemyGeneration++;
		if (enemyGeneration > 50)
		{
			enemyGeneration = 0;
		}

		enemyStraightsGen++;
		if (enemyStraightsGen > 25)
		{
			enemyStraightsGen = 0;
		}
		enemyCirclesGen++;
		if (enemyCirclesGen > 180)
		{
			enemyCirclesGen = 0;
		}
#pragma endregion

#pragma region 敵の更新処理

		for (const std::unique_ptr<Enemy>& enemy : enemys)
		{
			enemy->Update();
		}

		for (const std::unique_ptr<EnemyStraight>& enemy : enemyStraights)
		{
			enemy->Update();
		}
		for (const std::unique_ptr<EnemyCircle>& enemy : enemyCircles)
		{
			enemy->Update();
		}
#pragma endregion

		MathUtility::MatrixCalculation(worldtransform_);//行列の更新
		worldtransform_.TransferMatrix();

		//MathUtility::MatrixCalculation(spawnEnemyCircle);//行列の更新
		//spawnEnemyCircle.TransferMatrix();

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

#pragma region 敵の描画処理

		for (const std::unique_ptr<Enemy>& enemy : enemys)
		{
			enemy->Draw(viewProjection);
		}

		for (const std::unique_ptr<EnemyStraight>& enemy : enemyStraights)
		{
			enemy->Draw(viewProjection);
		}
		for (const std::unique_ptr<EnemyCircle>& enemy : enemyCircles)
		{
			enemy->Draw(viewProjection);
		}

#pragma endregion

		score->Draw();

#pragma region スポーン地点の描画

		spawn_->Draw(worldtransform_, viewProjection, texture);
		/*spawn_->Draw(spawnEnemyCircle, viewProjection, texture);*/
#pragma endregion

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

#pragma region マップ関連

		map->Draw();
		map->EnemyDraw(enemys);
		map->EnemyStraightDraw(enemyStraights);
		map->EnemyCircleDraw(enemyCircles);

#pragma endregion

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
void GameScene::EnemyOcurrence(const myMath::Vector3& p) {

	myMath::Vector3 position = { p.x,p.y,p.z };
	//Enemyを生成し、初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(model, position, texture);
	//Enemyを登録する
	enemys.push_back(std::move(newEnemy));
}

//生成関数を呼ぶ為のもの
void GameScene::EnemySpawn(const myMath::Vector3& p)
{
	if (enemys.size() < 100)
	{
		if (enemyGeneration == 0)
		{
			EnemyOcurrence(p);
		}
	}
}

void GameScene::EnemyStraightsGen(const myMath::Vector3& p,float angle)
{
	myMath::Vector3 position = { p.x,p.y,p.z };
	//Enemyを生成し、初期化
	std::unique_ptr<EnemyStraight> newEnemy = std::make_unique<EnemyStraight>();
	newEnemy->Initialize(viewProjection, position, angle);
	//Enemyを登録する
	enemyStraights.push_back(std::move(newEnemy));
}

void GameScene::EnemyStraightsSpawn(const myMath::Vector3& p, float angle)
{
	if (enemyStraightsGen ==0)
	{
		EnemyStraightsGen(p, angle);
	}
}

void GameScene::EnemyCirclesGen(const myMath::Vector3& p, float angle)
{
	myMath::Vector3 position = { p.x,p.y,p.z };
	//Enemyを生成し、初期化
	std::unique_ptr<EnemyCircle> newEnemy = std::make_unique<EnemyCircle>();
	newEnemy->Initialize(viewProjection, position, angle);
	//Enemyを登録する
	enemyCircles.push_back(std::move(newEnemy));
}

void GameScene::EnemyCirclesSpawn(const myMath::Vector3& p, float angle)
{
	if (enemyCircles.size() < 8)
	{
		if (enemyCirclesGen == 0)
		{
			EnemyCirclesGen(p, angle);
		}
	}
	
}
