#include "GameScene.h"
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
	
	titleTextrue_ = TextureManager::Load("bombTale_logo.png");
	spriteTielelogo_ = Sprite::Create(titleTextrue_, { 1280 / 2,170 }, { 1,1,1,1 }, { (0.5f),(0.5f) });
	
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

#pragma region 演出処理

		rot = spriteTielelogo_->GetRotation();
		pos = { spriteTielelogo_->GetPosition().x,spriteTielelogo_->GetPosition().y };

		if (isControl == true)
		{
			rot += 0.5f;
			pos.x += 20.0f;
			pos.y += 3.0f;
		}

		spriteTielelogo_->SetRotation(rot);
		spriteTielelogo_->SetPosition({ pos.x,pos.y });

		if (input_->TriggerKey(DIK_SPACE)) {
			isStart = true;
		}
#pragma endregion

		if (player->GetPlayerWorldTransform().translation_.z <= -100)
		{
			AudioManager::GetInstance()->StopWave(titleScene);//タイトルシーンのBGMを止める
			AudioManager::GetInstance()->PlayWave(gameScene, true);//ゲームシーンのBGMを再生

			Reset();
			scene = Game;	//ゲームシーンへ
		}

#pragma region マップ関連

		//床の行列更新
		MathUtility::MatrixCalculation(floorWorldTransform);//行列の更新
		floorWorldTransform.TransferMatrix();

#pragma endregion

		player->titleSceneUpdate();
		viewProjection.eye = { 0,75,-70 };
		viewProjection.target = { 0,0,-50 };

		if (player->GetTimer() == 33 && isControl == false)
		{
			explosionTransform = player->GetTaleWorldTransform();
			explosionTransform.scale_ = player->GetAttackWorldTransform().scale_;
			explosionTransform.scale_.x *= 2;
			explosionTransform.scale_.y *= 2;
			explosionTransform.scale_.z *= 2;
			isExplosion = true;
			isAnimation = true;
			isControl = true;
		}
		if (isAnimation == true)
		{
			for (int i = 0; i < 20; i++) {
				std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
				newobj->Initialize(player, explosionTransform, boxModel, orangeTexture_, 2);
				effects_.push_back(std::move(newobj));
			}
			isAnimation = false;
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

		MathUtility::MatrixCalculation(effectWorldTransform);//行列の更新
		effectWorldTransform.TransferMatrix();

		effects_.remove_if([](std::unique_ptr<Effect>& obj_) {
			return obj_->IsDead();
			});

		for (std::unique_ptr<Effect>& object : effects_) {
			object->Update();
		}

		MathUtility::MatrixCalculation(explosionTransform);//行列の更新
		explosionTransform.TransferMatrix();

		viewProjection.TransferMatrix();
		viewProjection.UpdateMatrix();
		break;

		//ゲームシーン
	case Game:
		isControl = false;

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


		worldtransform_.translation_.x = spawnRightTop.x;
		worldtransform_.translation_.z = spawnRightTop.z;

		

#pragma endregion


		SpawnEmerge();

		SpawnCollider();


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
					effects_.push_back(std::move(newobj));
				}
			}
			//大爆発の場合ブロックを増やす
			else {
				for (int i = 0; i < 20; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, explosionTransform, boxModel, orangeTexture_, 2);
					effects_.push_back(std::move(newobj));
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
					effects_.push_back(std::move(newobj));
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
					effects_.push_back(std::move(newobj));
				}
				isAnimation = false;
			}
		}
		for (const std::unique_ptr<Enemy>& enemy : enemys) {
			if (enemy->GetIsDead() == true) {
				effectWorldTransform = enemy->GetWorldTransform();
				isAnimation = true;
			}

			if (isAnimation == true) {
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, redTexture_, 0);
					effects_.push_back(std::move(newobj));
				}
				isAnimation = false;
			}
		}

		MathUtility::MatrixCalculation(effectWorldTransform);//行列の更新
		effectWorldTransform.TransferMatrix();

		effects_.remove_if([](std::unique_ptr<Effect>& obj_) {
			return obj_->IsDead();
		});

		for (std::unique_ptr<Effect>& object : effects_) {
			object->Update();
		}

		MathUtility::MatrixCalculation(explosionTransform);//行列の更新
		explosionTransform.TransferMatrix();

#pragma endregion

#pragma region 敵の削除処理
		//デスフラグが立った敵を削除
		enemys.remove_if([](std::unique_ptr<Enemy>& enemy_) { return enemy_->GetIsDead(); });
		enemys.remove_if([](std::unique_ptr<Enemy>& enemy_) { return enemy_->GetWallHitIsDead(); });

		enemyStraights.remove_if([](std::unique_ptr<EnemyStraight>& enemy_) { return enemy_->GetIsDead(); });
		enemyStraights.remove_if([](std::unique_ptr<EnemyStraight>& enemy_) { return enemy_->GetWallHitIsDead(); });

		enemyCircles.remove_if([](std::unique_ptr<EnemyCircle>& enemy_) { return enemy_->GetIsDead(); });
		enemyCircles.remove_if([](std::unique_ptr<EnemyCircle>& enemy_) { return enemy_->GetWallHitIsDead(); });

		enemyBombs.remove_if([](std::unique_ptr<EnemyBomb>& enemy_) { return enemy_->GetIsDead(); });
		//enemyBombs.remove_if([](std::unique_ptr<EnemyBomb>& enemy_) { return enemy_->GetWallHitIsDead(); });

#pragma endregion

		score->Update();

#pragma region 敵の生成処理

		//EnemySpawn(spawnRightTop);	//右

		enemyStraightAngle += 0.02f;
		if (enemyStraightAngle > 2 * MathUtility::PI)
		{
			enemyStraightAngle = 0;//オーバーフロー回避処理
		}
		else if (enemyStraightAngle < 0)
		{
			enemyStraightAngle = 2 * MathUtility::PI;//オーバーフロー回避処理
		}
		//EnemyStraightsSpawn({ 0,0,0 }, enemyStraightAngle);


		//enemyCircleAngle -= 0.2f;
		//if (enemyCircleAngle > 2 * MathUtility::PI)
		//{
		//	enemyCircleAngle = 0;//オーバーフロー回避処理
		//}
		//else if (enemyCircleAngle < 0)
		//{
		//	enemyCircleAngle = 2 * MathUtility::PI;//オーバーフロー回避処理
		//}
		//EnemyCirclesSpawn({ 20,0,20 }, enemyCircleAngle);

		//EnemyBombsSpawn({10,0,10});

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
		if (enemyGeneration > 120)
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

#pragma region  フィールドマップの描画

		floorModel->Draw(floorWorldTransform, viewProjection);

		for (int i = 0; i < 4; i++)
		{
			wallModel->Draw(wallWorldTransform[i], viewProjection);
		}
#pragma endregion

		for (std::unique_ptr<Effect>& object : effects_) {
			object->Draw(viewProjection);
		}

		if (isExplosion == true) {
			exModel_->Draw(explosionTransform, viewProjection, orangeTexture_);
		}

		player->Draw(viewProjection);

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


		for (std::unique_ptr<Effect>& object : effects_) {
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
	debugText_->SetPos(20, 200);
	debugText_->Printf("num %d", spawnNum);
	
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
	case Title:
		spriteTielelogo_->Draw();

		break;
	case Game:
		score->Draw();

#pragma region マップ関連

		map->Draw();
		map->EnemyDraw(enemys);
		map->EnemyStraightDraw(enemyStraights);
		map->EnemyCircleDraw(enemyCircles);

#pragma endregion

		break;

	case Result:
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
	
	
	if (enemyStraightsGen == 0)
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
	effects_.clear();
	isStart = false;
	pos.x = 1280/2;
	pos.y = 170;
	rot = 0;
	spriteTielelogo_->SetRotation(rot);
	spriteTielelogo_->SetPosition({ pos.x,pos.y });
#pragma region スポーン関連のリセット
	isSpawnLeftTop = false;
	isSpawnMiddleTop = false;
	isSpawnRightTop = false;
	isSpawnLeftCenter = false;
	isSpawnMiddleCenter = false;
	isSpawnRightCenter = false;
	isSpawnLeftBottom = false;
	isSpawnMiddleBottom = false;
	isSpawnRightBottom = false;
	isSpawnLMTop = false;
	isSpawnLMBottom = false;
	isSpawnRMTop = false;
	isSpawnRMBottom = false;

	spawnTimer = 300;
	spawnNum = 0;

#pragma endregion


}

void GameScene::SpawnCollider()
{
	if (player->GetAttackFlag())
	{
		//真ん中との当たり判定
		if (isSpawnMiddleTop == true)
		{			
			if ((2.0f + player->GetBombCharge()) * (2.0f + player->GetBombCharge()) >= (spawnMiddleTop.x - player->GetAttackWorldTransform().translation_.x) * (spawnMiddleTop.x - player->GetAttackWorldTransform().translation_.x) + (spawnMiddleTop.z - player->GetAttackWorldTransform().translation_.z) * (spawnMiddleTop.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnMiddleTop = false;
				spawnCount--;
			}
		}
		if (isSpawnMiddleCenter == true)
		{
			if ((2.0f + player->GetBombCharge()) * (2.0f + player->GetBombCharge()) >= (spawnMiddleCenter.x - player->GetAttackWorldTransform().translation_.x) * (spawnMiddleCenter.x - player->GetAttackWorldTransform().translation_.x) + (spawnMiddleCenter.z - player->GetAttackWorldTransform().translation_.z) * (spawnMiddleCenter.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnMiddleCenter = false;
				spawnCount--;
			}
		}
		if (isSpawnMiddleBottom == true)
		{
			if ((2.0f + player->GetBombCharge()) * (2.0f + player->GetBombCharge()) >= (spawnMiddleBottom.x - player->GetAttackWorldTransform().translation_.x) * (spawnMiddleBottom.x - player->GetAttackWorldTransform().translation_.x) + (spawnMiddleBottom.z - player->GetAttackWorldTransform().translation_.z) * (spawnMiddleBottom.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnMiddleBottom = false;
				spawnCount--;
			}
		}	

		//左との当たり判定
		if (isSpawnLeftTop == true)
		{
			if ((2.0f + player->GetBombCharge()) * (2.0f + player->GetBombCharge()) >= (spawnLeftTop.x - player->GetAttackWorldTransform().translation_.x) * (spawnLeftTop.x - player->GetAttackWorldTransform().translation_.x) + (spawnLeftTop.z - player->GetAttackWorldTransform().translation_.z) * (spawnLeftTop.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnLeftTop = false;
				spawnCount--;
			}
		}
		if (isSpawnLeftCenter == true)
		{
			if ((1.0f + player->GetBombCharge()) * (1.0f + player->GetBombCharge()) >= (spawnLeftCenter.x - player->GetAttackWorldTransform().translation_.x) * (spawnLeftCenter.x - player->GetAttackWorldTransform().translation_.x) + (spawnLeftCenter.z - player->GetAttackWorldTransform().translation_.z) * (spawnLeftCenter.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnLeftCenter = false;
				spawnCount--;
			}
		}
		if (isSpawnLeftBottom == true)
		{
			if ((2.0f + player->GetBombCharge()) * (2.0f + player->GetBombCharge()) >= (spawnLeftBottom.x - player->GetAttackWorldTransform().translation_.x) * (spawnLeftBottom.x - player->GetAttackWorldTransform().translation_.x) + (spawnLeftBottom.z - player->GetAttackWorldTransform().translation_.z) * (spawnLeftBottom.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnLeftBottom = false;
				spawnCount--;
			}
		}
		
		//右との当たり判定
		if (isSpawnRightTop == true)
		{
			if ((2.0f + player->GetBombCharge()) * (2.0f + player->GetBombCharge()) >= (spawnRightTop.x - player->GetAttackWorldTransform().translation_.x) * (spawnRightTop.x - player->GetAttackWorldTransform().translation_.x) + (spawnRightTop.z - player->GetAttackWorldTransform().translation_.z) * (spawnRightTop.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnRightTop = false;
				spawnCount--;
			}
		}
		if (isSpawnRightCenter == true)
		{
			if ((2.0f + player->GetBombCharge()) * (2.0f + player->GetBombCharge()) >= (spawnRightCenter.x - player->GetAttackWorldTransform().translation_.x) * (spawnRightCenter.x - player->GetAttackWorldTransform().translation_.x) + (spawnRightCenter.z - player->GetAttackWorldTransform().translation_.z) * (spawnRightCenter.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnRightCenter = false;
				spawnCount--;
			}
		}
		
		if (isSpawnRightBottom == true)
		{
			if ((2.0f + player->GetBombCharge()) * (2.0f + player->GetBombCharge()) >= (spawnRightBottom.x - player->GetAttackWorldTransform().translation_.x) * (spawnRightBottom.x - player->GetAttackWorldTransform().translation_.x) + (spawnRightBottom.z - player->GetAttackWorldTransform().translation_.z) * (spawnRightBottom.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnRightBottom = false;
				spawnCount--;
			}
		}
		
		//各間の当たり判定
		if (isSpawnLMTop == true)
		{
			if ((2.0f + player->GetBombCharge()) * (2.0f + player->GetBombCharge()) >= (spawnLMTop.x - player->GetAttackWorldTransform().translation_.x) * (spawnLMTop.x - player->GetAttackWorldTransform().translation_.x) + (spawnLMTop.z - player->GetAttackWorldTransform().translation_.z) * (spawnLMTop.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnLMTop = false;
				spawnCount--;
			}
		}
		if (isSpawnLMBottom == true)
		{
			if ((2.0f + player->GetBombCharge()) * (2.0f + player->GetBombCharge()) >= (spawnLMBottom.x - player->GetAttackWorldTransform().translation_.x) * (spawnLMBottom.x - player->GetAttackWorldTransform().translation_.x) + (spawnLMBottom.z - player->GetAttackWorldTransform().translation_.z) * (spawnLMBottom.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnLMBottom = false;
				spawnCount--;
			}
		}
		if (isSpawnRMTop == true)
		{
			if ((2.0f + player->GetBombCharge()) * (2.0f + player->GetBombCharge()) >= (spawnRMTop.x - player->GetAttackWorldTransform().translation_.x) * (spawnRMTop.x - player->GetAttackWorldTransform().translation_.x) + (spawnRMTop.z - player->GetAttackWorldTransform().translation_.z) * (spawnRMTop.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnRMTop = false;
				spawnCount--;
			}
		}
		if (isSpawnRMBottom == true)
		{
			if ((2.0f + player->GetBombCharge()) * (2.0f + player->GetBombCharge()) >= (spawnRMBottom.x - player->GetAttackWorldTransform().translation_.x) * (spawnRMBottom.x - player->GetAttackWorldTransform().translation_.x) + (spawnRMBottom.z - player->GetAttackWorldTransform().translation_.z) * (spawnRMBottom.z - player->GetAttackWorldTransform().translation_.z))
			{
				score->SetTimer(score->GetTimer() + 5);
				isSpawnRMBottom = false;
				spawnCount--;
			}
		}
	}
}

void GameScene::SpawnEmerge()
{
#pragma region スポーン地点の確定


	spawnTimer--;
	srand(time(nullptr));
	for (int i = 13; i <= 13; i++)
	{
		if (spawnTimer <= 0)
		{

			if (spawnCount < 13)
			{
				spawnNum = rand() % 13 + 1;//スポーン地点の出現をランダムで決定
			}
		

		}
		if (spawnNum == 1)
		{
			if (isSpawnLeftTop == false)
			{
				isSpawnLeftTop = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}
		}
		
		if (spawnNum == 2)
		{
			if (isSpawnMiddleTop == false)
			{
				isSpawnMiddleTop = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 3)
		{
			if (isSpawnRightTop == false)
			{
				isSpawnRightTop = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 4)
		{
			if (isSpawnLeftCenter == false)
			{
				isSpawnLeftCenter = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 5)
		{
			if (isSpawnMiddleCenter == false)
			{
				isSpawnMiddleCenter = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 6)
		{
			if (isSpawnRightCenter == false)
			{
				isSpawnRightCenter = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 7)
		{
			if (isSpawnLeftBottom == false)
			{
				isSpawnLeftBottom = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 8)
		{
			if (isSpawnMiddleBottom == false)
			{
				isSpawnMiddleBottom = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 9)
		{

			if (isSpawnRightBottom == false)
			{
				isSpawnRightBottom = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 10)
		{
			if (isSpawnLMTop == false)
			{
				isSpawnLMTop = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 11)
		{
			if (isSpawnLMBottom == false)
			{
				isSpawnLMBottom = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 12)
		{
			if (isSpawnRMTop == false)
			{
				isSpawnRMTop = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}

		}
		if (spawnNum == 13)
		{
			if (isSpawnRMBottom == false)
			{
				isSpawnRMBottom = true;
				spawnTimer = 300;
				spawnCount++;
			}
			else
			{
				continue;
			}
		}
	}

#pragma endregion

#pragma region 各スポーン地点の復活
	if (isSpawnLeftTop == true)
	{
		EnemySpawn(spawnLeftTop);
	}

	if (isSpawnMiddleTop == true)
	{
		EnemyStraightsSpawn(spawnMiddleTop, enemyStraightAngle);
	}

	if (isSpawnRightTop == true)
	{
		EnemyStraightsSpawn(spawnRightTop, enemyStraightAngle);
	}

	if (isSpawnLeftCenter == true)
	{
		EnemySpawn(spawnLeftCenter);
	}

	if (isSpawnMiddleCenter == true)
	{
		EnemyStraightsSpawn(spawnMiddleCenter, enemyStraightAngle);
	}

	if (isSpawnRightCenter == true)
	{
		EnemyStraightsSpawn(spawnRightCenter, enemyStraightAngle);
	}

	if (isSpawnLeftBottom == true)
	{
		EnemySpawn(spawnLeftBottom);
	}

	if (isSpawnMiddleBottom == true)
	{
		EnemyStraightsSpawn(spawnMiddleBottom, enemyStraightAngle);
	}

	if (isSpawnRightBottom == true)
	{
		EnemyStraightsSpawn(spawnRightBottom, enemyStraightAngle);
	}

	if (isSpawnLMTop == true)
	{
		EnemyStraightsSpawn(spawnLMTop, enemyStraightAngle);
	}
	if (isSpawnLMBottom == true)
	{
		EnemyStraightsSpawn(spawnLMBottom, enemyStraightAngle);
	}
	if (isSpawnRMTop == true)
	{
		EnemyStraightsSpawn(spawnRMTop, enemyStraightAngle);
	}
	if (isSpawnRMBottom == true)
	{
		EnemyStraightsSpawn(spawnRMBottom, enemyStraightAngle);
	}

#pragma endregion 
}
