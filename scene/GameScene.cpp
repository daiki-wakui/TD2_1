﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete floorModel;
	delete wallModel;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audioManager = audioManager->GetInstance();
	debugText_ = DebugText::GetInstance();
	model = Model::CreateFromOBJ("enemy", true);
	bombModel = Model::CreateFromOBJ("enemy_bomb",true);
	viewProjection.eye = { 0,75,-10 };
	viewProjection.Initialize();
	oldViewProjection.eye = { 0,75,-10 };
	oldViewProjection.Initialize();
	boxModel = Model::Create();

	player = Player::GetInstance();
	player->Initialize(viewProjection);

	texture = TextureManager::Load("mario.jpg");
	redTexture_ = TextureManager::Load("red.png");
	whiteTexture_ = TextureManager::Load("wit.png");
	orangeTexture_ = TextureManager::Load("orange2.png");
	
	
	map = std::make_unique<Map>();
	map->Initialize(viewProjection);

#pragma region フィールドマップの初期化とモデル読み込み

	//床
	floorModel = Model::CreateFromOBJ("floor", true);
	floorWorldTransform.scale_ = { 15.0f,10.0f,10.0f };
	floorWorldTransform.Initialize();

	//壁
	wallModel = Model::CreateFromOBJ("wall", true);
	//上壁
	wallWorldTransform[0].scale_ = {5.0f,5.0f,30.0f};
	wallWorldTransform[0].rotation_ = {0.0f,1.57f,0.0f};
	wallWorldTransform[0].translation_ = {0.0f,0.0f,97.0f};
	//下壁
	wallWorldTransform[1].scale_ = { 5.0f,5.0f,30.0f };
	wallWorldTransform[1].rotation_ = { 0.0f,1.57f,0.0f };
	wallWorldTransform[1].translation_ = { 0.0f,0.0f,-97.0f };
	//右壁
	wallWorldTransform[2].scale_ = { 5.0f,5.0f,19.5f };
	wallWorldTransform[2].translation_ = { 148.0f,0.0f,0.0f };
	//左壁
	wallWorldTransform[3].scale_ = { 5.0f,5.0f,19.5f };
	wallWorldTransform[3].translation_ = { -148.0f,0.0f,0.0f };

	for (int i = 0; i < 4; i++) {
		wallWorldTransform[i].Initialize();
	}

	//爆発モデル
	exModel_ = Model::CreateFromOBJ("ex", true);
	explosionTransform.Initialize();


	effectWorldTransform.translation_ = { 50,0,0 };
	effectWorldTransform.Initialize();
	World.Initialize();

#pragma endregion

	score = Score::GetInstance();
	score->Initialize();

#pragma region スポーン地点の生成

	spawn_ = Model::Create();
	worldtransform_.scale_ = { 2.0f,2.0f,2.0f };
	worldtransform_.Initialize();

	/*spawnEnemyCircle.scale_ = { 2.0f,2.0f,2.0f };
	spawnEnemyCircle.Initialize();*/

#pragma endregion

#pragma region BGM初期化

	titleScene = AudioManager::GetInstance()->LoadAudio("Resources/title.mp3");//タイトルシーンBGM読み込み
	AudioManager::GetInstance()->PlayWave(titleScene, true);//タイトルシーンのBGMを再生
	AudioManager::GetInstance()->ChangeVolume(titleScene,0.2f);
	gameScene = AudioManager::GetInstance()->LoadAudio("Resources/game.mp3");//ゲームシーンBGM読み込み
	AudioManager::GetInstance()->ChangeVolume(gameScene, 0.2f);

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
			AudioManager::GetInstance()->StopWave(titleScene);//タイトルシーンのBGMを止める
			AudioManager::GetInstance()->PlayWave(gameScene, true);//ゲームシーンのBGMを再生
		
			scene = Game;	//ゲームシーンへ
		}

		break;

		//ゲームシーン
	case Game:

		if (score->IsFinish())
		{
			AudioManager::GetInstance()->StopWave(gameScene);//タイトルシーンのBGMを止める
			scene = Result;//リザルトシーン
		}

		if (input_->TriggerKey(DIK_RETURN))
		{
			AudioManager::GetInstance()->StopWave(gameScene);
			scene = Result;
		}
#pragma region リセット処理

		if (input_->TriggerKey(DIK_R))
		{
			Reset();
		}

#pragma endregion

		player->Update();
		viewProjection.eye = { player->GetPlayerWorldTransform().translation_.x,75,player->GetPlayerWorldTransform().translation_.z - 20 };
		viewProjection.target = { player->GetPlayerWorldTransform().translation_.x,0,player->GetPlayerWorldTransform().translation_.z };

#pragma region worldTransformにスポーン地点の座標代入

		spawnRightTopPos = { 125.0f,0.0f,75.0f };

		worldtransform_.translation_.x = spawnRightTopPos.x;
		worldtransform_.translation_.z = spawnRightTopPos.z;

		spawnCenterPos = { 0,0,0 };
		spawnEnemyCircle.translation_.x = spawnCenterPos.x;
		spawnEnemyCircle.translation_.z = spawnCenterPos.z;

#pragma endregion


#pragma region  エフェクト関連の更新処理

		//playerの爆発エフェクト
		if (input_->TriggerKey(DIK_SPACE)) {
			explosionTransform = player->GetTaleWorldTransform();
			explosionTransform.scale_ = player->GetAttackWorldTransform().scale_;
			explosionTransform.scale_.x *= 2;
			explosionTransform.scale_.y *= 2;
			explosionTransform.scale_.z *= 2;

			isExplosion = true;

			//小爆発の場合散らばるブロックを減らす
			if (player->GetAttackWorldTransform().scale_.x < 4.0f) {
				for (int i = 0; i < 5; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, explosionTransform, boxModel, orangeTexture_, 2);
					objs_.push_back(std::move(newobj));
				}
			}
			//大爆発の場合ブロックを増やす
			else {
				for (int i = 0; i < 20; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, explosionTransform, boxModel, orangeTexture_, 2);
					objs_.push_back(std::move(newobj));
				}
			}
		}
		//爆発したら
		if (isExplosion == true) {
			explosionTransform.scale_.x -= 1.5f;
			explosionTransform.scale_.y -= 1.5f;
			explosionTransform.scale_.z -= 1.5f;

			if (explosionTransform.scale_.x < 0) {
				isExplosion = false;
			}
		}

		//敵が死ぬ時の演出
		for (const std::unique_ptr<EnemyCircle>& enemycir : enemyCircles) {
			if (enemycir->GetIsDead() == true) {
				effectWorldTransform = enemycir->GetWorldTransform();
				isAnimation = true;
			}

			if (isAnimation == true) {
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player,effectWorldTransform, boxModel, redTexture_, 0);
					objs_.push_back(std::move(newobj));
				}
				isAnimation = false;
			}
		}
		for (const std::unique_ptr<EnemyStraight>& enemystr : enemyStraights) {
			if (enemystr->GetIsDead() == true) {
				effectWorldTransform = enemystr->GetWorldTransform();
				isAnimation = true;
			}

			if (isAnimation == true) {
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player,effectWorldTransform, boxModel, redTexture_, 0);
					objs_.push_back(std::move(newobj));
				}
				isAnimation = false;
			}
		}

		MathUtility::MatrixCalculation(effectWorldTransform);//行列の更新
		effectWorldTransform.TransferMatrix();

		objs_.remove_if([](std::unique_ptr<Effect>& obj_) {
			return obj_->IsDead();
		});

		for (std::unique_ptr<Effect>& object : objs_) {
			object->Update();
		}

		MathUtility::MatrixCalculation(explosionTransform);//行列の更新
		explosionTransform.TransferMatrix();

#pragma endregion

#pragma region 敵の削除処理
		//デスフラグが立った敵を削除
		enemys.remove_if([](std::unique_ptr<Enemy>& enemy_) { return enemy_->GetIsDead(); });
		enemyStraights.remove_if([](std::unique_ptr<EnemyStraight>& enemy_) { return enemy_->GetIsDead(); });
		enemyCircles.remove_if([](std::unique_ptr<EnemyCircle>& enemy_) { return enemy_->GetIsDead(); });
		enemyBombs.remove_if([](std::unique_ptr<EnemyBomb>& enemy_) { return enemy_->GetIsDead(); });
#pragma endregion

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

		EnemyBombsSpawn({10,0,10});

#pragma endregion

#pragma region マップ関連

		//床の行列更新
		MathUtility::MatrixCalculation(floorWorldTransform);//行列の更新
		floorWorldTransform.TransferMatrix();

		//壁の行列更新
		for (int i = 0; i < 4; i++) 
		{
			MathUtility::MatrixCalculation(wallWorldTransform[i]);//行列の更新
			wallWorldTransform[i].TransferMatrix();
		}

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
		enemyBombsGen++;
		if (enemyBombsGen > 180)
		{
			enemyBombsGen = 0;
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
		for (const std::unique_ptr<EnemyBomb>& enemy : enemyBombs)
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
		
		if (input_->TriggerKey(DIK_SPACE))
		{
			Reset();
			AudioManager::GetInstance()->PlayWave(titleScene, true);//タイトルシーンのBGMを再生
			scene = Title;
		}

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

#pragma region  フィールドマップの描画

		floorModel->Draw(floorWorldTransform, viewProjection);

		for (int i = 0; i < 4; i++)
		{
			wallModel->Draw(wallWorldTransform[i], viewProjection);
		}
#pragma endregion

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
		for (const std::unique_ptr<EnemyBomb>& enemy : enemyBombs)
		{
			enemy->Draw(viewProjection);
		}
#pragma endregion

		

#pragma region スポーン地点の描画

		spawn_->Draw(worldtransform_, viewProjection, texture);
		/*spawn_->Draw(spawnEnemyCircle, viewProjection, texture);*/
#pragma endregion


		for (std::unique_ptr<Effect>& object : objs_) {
			object->Draw(viewProjection);
		}

		if (isExplosion == true) {
			exModel_->Draw(explosionTransform, viewProjection, orangeTexture_);
		}

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
		score->Draw();
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
	newEnemy->Initialize(model,viewProjection, position, angle);
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
	newEnemy->Initialize(model,viewProjection, position, angle);
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

void GameScene::EnemyBombsGen(const myMath::Vector3& p)
{
	myMath::Vector3 position = { p.x,p.y,p.z };
	//Enemyを生成し、初期化
	std::unique_ptr<EnemyBomb> newEnemy = std::make_unique<EnemyBomb>();
	newEnemy->Initialize(bombModel, viewProjection, position);
	//Enemyを登録する
	enemyBombs.push_back(std::move(newEnemy));
}

void GameScene::EnemyBombsSpawn(const myMath::Vector3& p)
{
	if (enemyBombs.size() < 3)
	{
		if (enemyBombsGen == 0)
		{
			EnemyBombsGen(p);
		}
	}

}

void GameScene::Reset()
{
	player->Reset();
	score->Reset();
	enemys.clear();
	enemyCircles.clear();
	enemyStraights.clear();
	enemyGeneration = 0;
	enemyStraightsGen = 0;
	enemyStraightAngle = 0;
	enemyCirclesGen = 0;
	enemyCircleAngle = 0;
}