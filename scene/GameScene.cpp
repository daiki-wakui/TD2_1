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
	bombModel = Model::CreateFromOBJ("enemy_bomb", true);
	viewProjection.eye = { 0,75,-10 };
	viewProjection.Initialize();
	oldViewProjection.eye = { 0,75,-10 };
	oldViewProjection.Initialize();
	boxModel = Model::Create();

	player = Player::GetInstance();
	player->Initialize(viewProjection);

	texture = TextureManager::Load("mario.jpg");
	redTexture_ = TextureManager::Load("colorTex/red.png");
	blueTexture_ = TextureManager::Load("colorTex/Blue.png");
	whiteTexture_ = TextureManager::Load("colorTex/wit.png");
	orangeTexture_ = TextureManager::Load("colorTex/orange2.png");
	purpleTexture_ = TextureManager::Load("colorTex/purple.png");

	titleUITex_ = TextureManager::Load("titlteStart.png");
	spriteTitleUI_ = Sprite::Create(titleUITex_, { 1280 / 2,550 }, { 1,1,1,1 }, { (0.5f),(0.5f) });
	titleTextrue_ = TextureManager::Load("bombTale_logo.png");
	spriteTielelogo_ = Sprite::Create(titleTextrue_, { 1280 / 2,170 }, { 1,1,1,1 }, { (0.5f),(0.5f) });

	tutorialTexture[0] = TextureManager::Load("tutorial/bombTale_explanation_01.png");
	tutorialTexture[1] = TextureManager::Load("tutorial/bombTale_explanation_02.png");
	tutorialUITex_ = TextureManager::Load("tutorial/tutorialNext.png");
	tutorialFinishTex_ = TextureManager::Load("tutorial/tutorialStart.png");
	spriteTutorial1 = Sprite::Create(tutorialTexture[0], { 1280 / 2,720/2 }, { 1,1,1,1 }, { (0.5f),(0.5f) });
	spriteTutorial2 = Sprite::Create(tutorialTexture[1], { 1280 / 2,720/2 }, { 1,1,1,1 }, { (0.5f),(0.5f) });
	spriteTutorialUI_ = Sprite::Create(tutorialUITex_, { 1075,620 }, { 1,1,1,1 }, { (0.5f),(0.5f) });
	spriteTutorialFinish_ = Sprite::Create(tutorialFinishTex_, { 1075,620 }, { 1,1,1,1 }, { (0.5f),(0.5f) });


	scsneChangeTexture_ = TextureManager::Load("colorTex/sceneChange.png");
	spriteSceneChange = Sprite::Create(scsneChangeTexture_, { 0,720 });
	
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
	wallWorldTransform[0].scale_ = { 5.0f,5.0f,30.0f };
	wallWorldTransform[0].rotation_ = { 0.0f,1.57f,0.0f };
	wallWorldTransform[0].translation_ = { 0.0f,0.0f,97.0f };
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

	mapSpawnerHandle = TextureManager::Load("mapPlayer.png");

	spawnWorldTransform[MiddleTop].translation_ = { spawnMiddleTop.x,spawnMiddleTop.y,spawnMiddleTop.z };
	spawnWorldTransform[MiddleCenter].translation_ = { spawnMiddleCenter.x,spawnMiddleCenter.y,spawnMiddleCenter.z };
	spawnWorldTransform[MiddleBottom].translation_ = { spawnMiddleBottom.x,spawnMiddleBottom.y,spawnMiddleBottom.z };
	spawnWorldTransform[LeftTop].translation_ = { spawnLeftTop.x,spawnLeftTop.y,spawnLeftTop.z };
	spawnWorldTransform[LeftCenter].translation_ = { spawnLeftCenter.x,spawnLeftCenter.y,spawnLeftCenter.z };
	spawnWorldTransform[LeftBottom].translation_ = { spawnLeftBottom.x,spawnLeftBottom.y,spawnLeftBottom.z };
	spawnWorldTransform[RightTop].translation_ = { spawnRightTop.x,spawnRightTop.y,spawnRightTop.z };
	spawnWorldTransform[RightCenter].translation_ = { spawnRightCenter.x,spawnRightCenter.y,spawnRightCenter.z };
	spawnWorldTransform[RightBottom].translation_ = { spawnRightBottom.x,spawnRightBottom.y,spawnRightBottom.z };
	spawnWorldTransform[LMTop].translation_ = { spawnLMTop.x,spawnLMTop.y,spawnLMTop.z };
	spawnWorldTransform[LMBottom].translation_ = { spawnLMBottom.x,spawnLMBottom.y,spawnLMBottom.z};
	spawnWorldTransform[RMTop].translation_ = { spawnRMTop.x,spawnRMTop.y,spawnRMTop.z};
	spawnWorldTransform[RMBottom].translation_ = { spawnRMBottom.x,spawnRMBottom.y,spawnRMBottom.z };

	for (int i = 0; i < 13; i++) {
		spawnWorldTransform[i].scale_ = { 2.5f,2.5f,2.5f };
		spawnWorldTransform[i].Initialize();
	}

#pragma endregion

	score = Score::GetInstance();
	score->Initialize();

#pragma region スポーン地点の生成

	spawn_ = Model::CreateFromOBJ("enemy_spawner", true);
	worldtransform_.scale_ = { 2.0f,2.0f,2.0f };
	worldtransform_.Initialize();

	


	/*spawnEnemyCircle.scale_ = { 2.0f,2.0f,2.0f };
	spawnEnemyCircle.Initialize();*/

#pragma endregion

#pragma region BGM初期化

	titleSceneBGM = audioManager->LoadAudio("Resources/Sound/title.mp3");//タイトルシーンBGM読み込み
	audioManager->PlayWave(titleSceneBGM, true);//タイトルシーンのBGMを再生
	audioManager->ChangeVolume(titleSceneBGM, 0.2f);
	gameSceneBGM = audioManager->LoadAudio("Resources/Sound/game.mp3");//ゲームシーンBGM読み込み
	audioManager->ChangeVolume(gameSceneBGM, 0.2f);
	tutorialSceneBGM= audioManager->LoadAudio("Resources/Sound/チュートリアル.mp3");//チュートリアルシーンBGM読み込み
	audioManager->ChangeVolume(tutorialSceneBGM, 0.2f);
	resultSceneBGM = audioManager->LoadAudio("Resources/Sound/Blinded.mp3");//リザルトシーンBGM読み込み
	audioManager->ChangeVolume(resultSceneBGM, 0.2f);

	damageSE = audioManager->LoadAudio("Resources/Sound/打撃6.mp3");//ダメージSE読み込み
	spawnerBreakSE = audioManager->LoadAudio("Resources/Sound/パンチで壁を破壊.mp3");//スポナー破壊SE読み込み

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

		logoRot = spriteTielelogo_->GetRotation();
		logoPos = { spriteTielelogo_->GetPosition().x,spriteTielelogo_->GetPosition().y };

		if (isControl == true)
		{
			logoRot += 0.5f;
			logoPos.x += 20.0f;
			logoPos.y += 3.0f;
		}

		spriteTielelogo_->SetRotation(logoRot);
		spriteTielelogo_->SetPosition({ logoPos.x,logoPos.y });

		if (input_->TriggerKey(DIK_SPACE)) {
			isStart = true;
		}
#pragma endregion

		if (player->GetPlayerWorldTransform().translation_.z <= -100)
		{
			sceneChangePos = { spriteSceneChange->GetPosition().x,spriteSceneChange->GetPosition().y };

			sceneChangePos.y -= 50.0f;

			spriteSceneChange->SetPosition({ sceneChangePos.x,sceneChangePos.y });

			if(sceneChangePos.y<=0)
			{
				audioManager->StopWave(titleSceneBGM);//チュートリアルシーンのBGMを止める
				audioManager->PlayWave(tutorialSceneBGM, true);//ゲームシーンのBGMを再生

				Reset();
				scene = Tutorial;	//チュートリアルシーンへ
			}
		}

#pragma region マップ関連

		//床の行列更新
		MathUtility::MatrixCalculation(floorWorldTransform);//行列の更新
		floorWorldTransform.TransferMatrix();

#pragma endregion

		player->titleSceneUpdate(scene);
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

		//チュートリアル
	case Tutorial:

		//シーン遷移処理
		sceneChangePos = { spriteSceneChange->GetPosition().x,spriteSceneChange->GetPosition().y };
		sceneChangePos.y -= 50.0f;
		spriteSceneChange->SetPosition({ sceneChangePos.x,sceneChangePos.y });

#pragma region チュートリアルでのplayer処理

		if (isTutorial == false) {
			player->Update(scene);
			viewProjection.eye = { player->GetPlayerWorldTransform().translation_.x,75,player->GetPlayerWorldTransform().translation_.z - 20 };
			viewProjection.target = { player->GetPlayerWorldTransform().translation_.x,0,player->GetPlayerWorldTransform().translation_.z };
			
			if (tutorialCount == 4) {
				tutorialFinishTime++;

				if (tutorialFinishTime >= 600 && isTutorialFinish == false) {
					isTutorialFinish = true;
					spriteSceneChange->SetPosition({ 0,720 });
				}

				if (isTutorialFinish == true) {

					if (spriteSceneChange->GetPosition().y <= 0) {
						audioManager->StopWave(tutorialSceneBGM);//チュートリアルシーンのBGMを止める
						audioManager->PlayWave(gameSceneBGM, true);//ゲームシーンのBGMを再生

						Reset();
						player->SetChargeSEFlag(true);
						scene = Game;	//チュートリアルシーンへ
					}
				}
			}
			

			//playerの爆発エフェクト
			if (input_->TriggerKey(DIK_SPACE)) {
				tutorialMoveChack++;

				if (tutorialMoveChack == 4) {
					tutorialUIpower = 0;
					isTutorial = true;
					tutorialCount++;
				}
				else {
					explosionTransform = player->GetTaleWorldTransform();
					explosionTransform.scale_ = player->GetAttackWorldTransform().scale_;
					explosionTransform.scale_.x *= 2;
					explosionTransform.scale_.y *= 2;
					explosionTransform.scale_.z *= 2;

					isExplosion = true;

					//小爆発の場合散らばるブロックを減らす
					if (player->GetAttackWorldTransform().scale_.x < 4.0f) {
						for (int i = 0; i < 2; i++) {
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
			}
			//爆発したら
			if (isExplosion == true) {
				explosionTransform.scale_.x -= 3.0f;
				explosionTransform.scale_.y -= 3.0f;
				explosionTransform.scale_.z -= 3.0f;

				if (explosionTransform.scale_.x < 0) {
					isExplosion = false;
				}
			}
		}

		effects_.remove_if([](std::unique_ptr<Effect>& obj_) {
			return obj_->IsDead();
			});

		for (std::unique_ptr<Effect>& object : effects_) {
			object->Update();
		}

		//床の行列更新
		MathUtility::MatrixCalculation(floorWorldTransform);//行列の更新
		floorWorldTransform.TransferMatrix();

		//壁の行列更新
		for (int i = 0; i < 4; i++)
		{
			MathUtility::MatrixCalculation(wallWorldTransform[i]);//行列の更新
			wallWorldTransform[i].TransferMatrix();
		}

		MathUtility::MatrixCalculation(explosionTransform);//行列の更新
		explosionTransform.TransferMatrix();

		viewProjection.TransferMatrix();
		viewProjection.UpdateMatrix();

#pragma endregion

		if (isTutorial == true) {
			if (input_->TriggerKey(DIK_SPACE)) {
				tutorialCount++;

				if (tutorialCount == 1|| tutorialCount == 4) {
					isTutorial = false;
				}
			}
		}

		if (tutorialCount == 1) {
			Vector2 UIpos = spriteTutorial1->GetPosition();
			tutorialUIpower++;
			UIpos.x += tutorialUIpower;

			if (UIpos.x >= 1000) {
				UIpos.x = 1000;
			}
			
			spriteTutorial1->SetPosition(UIpos);
		}

		if (tutorialCount == 4) {
			Vector2 UIpos = spriteTutorial2->GetPosition();
			tutorialUIpower++;
			UIpos.x += tutorialUIpower;

			if (UIpos.x >= 1000) {
				UIpos.x = 1000;
			}

			spriteTutorial2->SetPosition(UIpos);
		}

		break;
		//ゲームシーン
	case Game:
		isControl = false;

		nowFlame++;
		if (nowFlame >= 60)
		{
			nowTime++;//経過時間
			nowFlame = 0;
		}

		sceneChangePos = { spriteSceneChange->GetPosition().x,spriteSceneChange->GetPosition().y };

		sceneChangePos.y -= 50.0f;

		spriteSceneChange->SetPosition({ sceneChangePos.x,sceneChangePos.y });

		if (score->IsFinish())
		{
			audioManager->StopWave(gameSceneBGM);//ゲームシーンのBGMを止める
			audioManager->PlayWave(resultSceneBGM, true);//リザルトシーンのBGMを再生する
			Reset();
			scene = Result;//リザルトシーン
		}

		if (input_->TriggerKey(DIK_RETURN))
		{
			audioManager->StopWave(gameSceneBGM);
			Reset();
			scene = Result;
		}
#pragma region リセット処理

		if (input_->TriggerKey(DIK_R))
		{
			Reset();
		}

#pragma endregion

		player->Update(scene);
		viewProjection.eye = { player->GetPlayerWorldTransform().translation_.x,75,player->GetPlayerWorldTransform().translation_.z - 20 };
		viewProjection.target = { player->GetPlayerWorldTransform().translation_.x,0,player->GetPlayerWorldTransform().translation_.z };

#pragma region worldTransformにスポーン地点の座標代入


		worldtransform_.translation_.x = spawnRightTop.x;
		worldtransform_.translation_.z = spawnRightTop.z;

		for (int i = 0; i < 13; i++) {
			MathUtility::MatrixCalculation(spawnWorldTransform[i]);
			spawnWorldTransform[i].TransferMatrix();
		}

#pragma endregion

		//

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
				for (int i = 0; i < 2; i++) {
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
			explosionTransform.scale_.x -= 3.0f;
			explosionTransform.scale_.y -= 3.0f;
			explosionTransform.scale_.z -= 3.0f;

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
			if (enemycir->GetPlayerHit() == true) {
				effectWorldTransform = enemycir->GetWorldTransform();
				isAnimation = 2;
			}

			if (isAnimation == 2) {
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, blueTexture_, 0);
					effects_.push_back(std::move(newobj));
				}
				isAnimation = false;
			}
			else if (isAnimation == true) {
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, redTexture_, 0);
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
			if (enemystr->GetPlayerHit() == true) {
				effectWorldTransform = enemystr->GetWorldTransform();
				isAnimation = 2;
			}

			if (isAnimation == 2) {
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, blueTexture_, 0);
					effects_.push_back(std::move(newobj));
				}
				isAnimation = false;
			}
			else if (isAnimation == true) {
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, redTexture_, 0);
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
			if (enemy->GetPlayerHit() == true) {
				effectWorldTransform = enemy->GetWorldTransform();
				isAnimation = 2;
			}

			if (isAnimation == 2) {
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, blueTexture_, 0);
					effects_.push_back(std::move(newobj));
				}
				isAnimation = false;
			}
			else if (isAnimation == true) {
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
		enemys.remove_if([](std::unique_ptr<Enemy>& enemy_) { return enemy_->GetPlayerHit(); });

		enemyStraights.remove_if([](std::unique_ptr<EnemyStraight>& enemy_) { return enemy_->GetIsDead(); });
		enemyStraights.remove_if([](std::unique_ptr<EnemyStraight>& enemy_) { return enemy_->GetWallHitIsDead(); });
		enemyStraights.remove_if([](std::unique_ptr<EnemyStraight>& enemy_) { return enemy_->GetPlayerHit(); });

		enemyCircles.remove_if([](std::unique_ptr<EnemyCircle>& enemy_) { return enemy_->GetIsDead(); });
		enemyCircles.remove_if([](std::unique_ptr<EnemyCircle>& enemy_) { return enemy_->GetWallHitIsDead(); });
		enemyCircles.remove_if([](std::unique_ptr<EnemyCircle>& enemy_) { return enemy_->GetPlayerHit(); });

		enemyBombs.remove_if([](std::unique_ptr<EnemyBomb>& enemy_) { return enemy_->GetIsDead(); });
		//enemyBombs.remove_if([](std::unique_ptr<EnemyBomb>& enemy_) { return enemy_->GetWallHitIsDead(); });

#pragma endregion

		score->Update();

#pragma region 敵の生成処理


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

#pragma endregion

#pragma region 敵の更新処理

		for (const std::unique_ptr<Enemy>& enemy : enemys)
		{
			enemy->Update(damageSE);
		}

		for (const std::unique_ptr<EnemyStraight>& enemy : enemyStraights)
		{
			enemy->Update(damageSE);
		}
		for (const std::unique_ptr<EnemyCircle>& enemy : enemyCircles)
		{
			enemy->Update(damageSE);
		}
		for (const std::unique_ptr<EnemyBomb>& enemy : enemyBombs)
		{
			enemy->Update();
		}

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
		map->EnemyStraightUpdate(enemyStraights, enemyStraightsGen);
		map->EnemyCircleUpdate(enemyCircles, enemyCirclesGen);

		if (isSpawn[LeftTop] == true)
		{
			mapSpawnerSprite[LeftTop] = Sprite::Create(mapSpawnerHandle, { 203 - 100.0f,153 - 50.0f }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

		if (isSpawn[MiddleTop] == true)
		{
			mapSpawnerSprite[MiddleTop] = Sprite::Create(mapSpawnerHandle, { 203.0f,153 - 50.0f }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

		if (isSpawn[RightTop] == true)
		{
			mapSpawnerSprite[RightTop] = Sprite::Create(mapSpawnerHandle, { 203 + 100.0f,153 - 50.0f }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

		if (isSpawn[LeftCenter] == true)
		{
			mapSpawnerSprite[LeftCenter] = Sprite::Create(mapSpawnerHandle, { 203 - 100.0f,153 }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

		if (isSpawn[MiddleCenter] == true)
		{
			mapSpawnerSprite[MiddleCenter] = Sprite::Create(mapSpawnerHandle, { 203,153 }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

		if (isSpawn[RightCenter] == true)
		{
			mapSpawnerSprite[RightCenter] = Sprite::Create(mapSpawnerHandle, { 203 + 100.0f,153 }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

		if (isSpawn[LeftBottom] == true)
		{
			mapSpawnerSprite[LeftBottom] = Sprite::Create(mapSpawnerHandle, { 203 - 100.0f,153+50.0f }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

		if (isSpawn[MiddleBottom] == true)
		{
			mapSpawnerSprite[MiddleBottom] = Sprite::Create(mapSpawnerHandle, { 203 ,153 +50.0f }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

		if (isSpawn[RightBottom] == true)
		{
			mapSpawnerSprite[RightBottom] = Sprite::Create(mapSpawnerHandle, { 203 + 100.0f,153 +50.0f }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

		if (isSpawn[LMTop] == true)
		{
			mapSpawnerSprite[LMTop] = Sprite::Create(mapSpawnerHandle, { 203 - 50.0f,153 - 25.0f }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

		if (isSpawn[LMBottom] == true)
		{
			mapSpawnerSprite[LMBottom] = Sprite::Create(mapSpawnerHandle, { 203 - 50.0f,153 +25.0f }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

		if (isSpawn[RMTop] == true)
		{
			mapSpawnerSprite[RMTop] = Sprite::Create(mapSpawnerHandle, { 203 + 50.0f ,153 - 25.0f }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

		if (isSpawn[RMBottom] == true)
		{
			mapSpawnerSprite[RMBottom] = Sprite::Create(mapSpawnerHandle, { 203 + 50.0f,153 + 25.0f }, { 1,0,1,1 }, { 0.5f,0.5f });
		}

#pragma endregion

#pragma region クールタイム処理
		//スポーンクールタイム
		enemyGeneration++;
		
		
		//追従敵
		if (nowTime < 30)
		{
			if (enemyGeneration > 420)
			{
				enemyGeneration = 0;
			}
		}
		else if (nowTime > 30 && nowTime < 60)
		{
			if (enemyGeneration > 300)
			{
				enemyGeneration = 0;
			}
		}
		else if (nowTime > 60)
		{
			if (enemyGeneration > 180)
			{
				enemyGeneration = 0;
			}
		}
		
		
		enemyStraightsGen++;
		

		if (nowTime < 30)
		{
			if (enemyStraightsGen > 75)
			{
				enemyStraightsGen = 0;
			}
		}
		else if (nowTime > 30 && nowTime < 60)
		{
			if (enemyStraightsGen > 50)
			{
				enemyStraightsGen = 0;
			}
		}
		else if(nowTime > 60)
		{
			if (enemyStraightsGen > 25)
			{
				enemyStraightsGen = 0;
			}
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

		MathUtility::MatrixCalculation(worldtransform_);//行列の更新
		worldtransform_.TransferMatrix();

		//MathUtility::MatrixCalculation(spawnEnemyCircle);//行列の更新
		//spawnEnemyCircle.TransferMatrix();

		viewProjection.TransferMatrix();
		viewProjection.UpdateMatrix();
		break;


		//リザルト画面
	case Result:

		if (input_->TriggerKey(DIK_SPACE))
		{
			sceneChangePos.x = 0;
			sceneChangePos.y = 720;
			spriteSceneChange->SetPosition({ sceneChangePos.x,sceneChangePos.y });
			Reset();
			audioManager->StopWave(resultSceneBGM);//リザルトシーンのBGMを止める
			audioManager->PlayWave(titleSceneBGM, true);//タイトルシーンのBGMを再生
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

		break;

		//チュートリアル
	case Tutorial:
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
		for (int i = 0; i < 13; i++) {

			if (isSpawn[i] == true) {
				spawn_->Draw(spawnWorldTransform[i], viewProjection);
			}
		}
		
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
		spriteSceneChange->Draw();
		if (isStart == false) {
			spriteTitleUI_->Draw();
		}
		

		break;

	case Tutorial:
		
		if (tutorialCount == 0 || tutorialCount == 1) {
			spriteTutorial1->Draw();
		}

		if (tutorialCount != 0 && tutorialCount != 1) {
			spriteTutorial2->Draw();
		}
		spriteSceneChange->Draw();

		if (tutorialCount == 0|| tutorialCount == 3) {
			spriteTutorialUI_->Draw();
		}
		

		debugText_->SetPos(20, 300);
		debugText_->Printf("tutorialCount %d", tutorialCount);

		debugText_->SetPos(20, 320);
		debugText_->Printf("movechack %d", tutorialMoveChack);

		debugText_->SetPos(20, 340);
		debugText_->Printf("time %d", tutorialFinishTime);
		break;

	case Game:
		score->Draw();

		spriteSceneChange->Draw();

		debugText_->SetPos(20, 40);
		debugText_->Printf("%d", spawnTimer);

		

#pragma region マップ関連

		map->Draw();
		map->EnemyDraw(enemys);
		map->EnemyStraightDraw(enemyStraights);
		map->EnemyCircleDraw(enemyCircles);

#pragma region マップのスポナーの描画

		if (isSpawn[LeftTop] == true)
		{
			mapSpawnerSprite[LeftTop]->Draw();
		}

		if (isSpawn[MiddleTop] == true)
		{
			mapSpawnerSprite[MiddleTop]->Draw();
		}

		if (isSpawn[RightTop] == true)
		{
			mapSpawnerSprite[RightTop]->Draw();
		}

		if (isSpawn[LeftCenter] == true)
		{
			mapSpawnerSprite[LeftCenter]->Draw();
		}

		if (isSpawn[MiddleCenter] == true)
		{
			mapSpawnerSprite[MiddleCenter]->Draw();
		}

		if (isSpawn[RightCenter] == true)
		{
			mapSpawnerSprite[RightCenter]->Draw();
		}

		if (isSpawn[LeftBottom] == true)
		{
			mapSpawnerSprite[LeftBottom]->Draw();
		}

		if (isSpawn[MiddleBottom] == true)
		{
			mapSpawnerSprite[MiddleBottom]->Draw();
		}

		if (isSpawn[RightBottom] == true)
		{
			mapSpawnerSprite[RightBottom]->Draw();
		}

		if (isSpawn[LMTop] == true)
		{
			mapSpawnerSprite[LMTop]->Draw();
		}

		if (isSpawn[LMBottom] == true)
		{
			mapSpawnerSprite[LMBottom]->Draw();
		}

		if (isSpawn[RMTop] == true)
		{
			mapSpawnerSprite[RMTop]->Draw();
		}

		if (isSpawn[RMBottom] == true)
		{
			mapSpawnerSprite[RMBottom]->Draw();
		}
		
#pragma endregion

		break;

	case Result:
		break;
	}

	/// </summary>

	// デバッグテキストの描画
	debugText_->SetPos(20, 60);
	debugText_->Printf("nowTime %d", nowTime);


	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//生成関数
void GameScene::EnemyOcurrence(const myMath::Vector3& p) {

	myMath::Vector3 position = { p.x,p.y,p.z };
	//Enemyを生成し、初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(model, position);
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

void GameScene::EnemyStraightsGen(const myMath::Vector3& p, float angle)
{
	myMath::Vector3 position = { p.x,p.y,p.z };
	//Enemyを生成し、初期化
	std::unique_ptr<EnemyStraight> newEnemy = std::make_unique<EnemyStraight>();
	newEnemy->Initialize(model, viewProjection, position, angle);
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
	newEnemy->Initialize(model, viewProjection, position, angle);
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
	logoPos.x = 1280/2;
	logoPos.y = 170;
	logoRot = 0;
	spriteTielelogo_->SetRotation(logoRot);
	spriteTielelogo_->SetPosition({ logoPos.x,logoPos.y });
	nowFlame = 0;
	nowTime = 0;
	isTutorial = true;
	isTutorialFinish = false;
	tutorialCount = 0;
	tutorialFinishTime = 0;
	tutorialMoveChack = 0;
	tutorialUIpower = 0;
	spriteTutorial1->SetPosition({ 1280 / 2,250 });
	spriteTutorial2->SetPosition({ 1280 / 2,250 });

	startSpawn = true;
#pragma region スポーン関連のリセット
	for (int i = 0; i < 13; i++) {
		isSpawn[i] = false;
	}

	spawnTimer = 0;
	spawnNum = 0;

#pragma endregion


}

void GameScene::SpawnCollider()
{
	if (player->GetAttackFlag())
	{
		//真ん中との当たり判定
		if (isSpawn[MiddleTop] == true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnMiddleTop.x - player->GetAttackWorldTransform().translation_.x) * (spawnMiddleTop.x - player->GetAttackWorldTransform().translation_.x) + (spawnMiddleTop.z - player->GetAttackWorldTransform().translation_.z) * (spawnMiddleTop.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[MiddleTop];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[MiddleTop] = false;
				spawnCount--;
			}
		}
		if (isSpawn[MiddleCenter] == true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnMiddleCenter.x - player->GetAttackWorldTransform().translation_.x) * (spawnMiddleCenter.x - player->GetAttackWorldTransform().translation_.x) + (spawnMiddleCenter.z - player->GetAttackWorldTransform().translation_.z) * (spawnMiddleCenter.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[MiddleCenter];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[MiddleCenter] = false;
				spawnCount--;
			}
		}
		if (isSpawn[MiddleBottom]== true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnMiddleBottom.x - player->GetAttackWorldTransform().translation_.x) * (spawnMiddleBottom.x - player->GetAttackWorldTransform().translation_.x) + (spawnMiddleBottom.z - player->GetAttackWorldTransform().translation_.z) * (spawnMiddleBottom.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[MiddleBottom];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[MiddleBottom] = false;
				spawnCount--;
			}
		}

		//左との当たり判定
		if (isSpawn[LeftTop] == true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnLeftTop.x - player->GetAttackWorldTransform().translation_.x) * (spawnLeftTop.x - player->GetAttackWorldTransform().translation_.x) + (spawnLeftTop.z - player->GetAttackWorldTransform().translation_.z) * (spawnLeftTop.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[LeftTop];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[LeftTop] = false;
				spawnCount--;
			}
		}
		if (isSpawn[LeftCenter] == true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnLeftCenter.x - player->GetAttackWorldTransform().translation_.x) * (spawnLeftCenter.x - player->GetAttackWorldTransform().translation_.x) + (spawnLeftCenter.z - player->GetAttackWorldTransform().translation_.z) * (spawnLeftCenter.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[LeftCenter];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[LeftCenter] = false;
				spawnCount--;
			}
		}
		if (isSpawn[LeftBottom] == true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnLeftBottom.x - player->GetAttackWorldTransform().translation_.x) * (spawnLeftBottom.x - player->GetAttackWorldTransform().translation_.x) + (spawnLeftBottom.z - player->GetAttackWorldTransform().translation_.z) * (spawnLeftBottom.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[LeftBottom];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[LeftBottom] = false;
				spawnCount--;
			}
		}

		//右との当たり判定
		if (isSpawn[RightTop] == true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnRightTop.x - player->GetAttackWorldTransform().translation_.x) * (spawnRightTop.x - player->GetAttackWorldTransform().translation_.x) + (spawnRightTop.z - player->GetAttackWorldTransform().translation_.z) * (spawnRightTop.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[RightTop];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[RightTop] = false;
				spawnCount--;
			}
		}
		if (isSpawn[RightCenter] == true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnRightCenter.x - player->GetAttackWorldTransform().translation_.x) * (spawnRightCenter.x - player->GetAttackWorldTransform().translation_.x) + (spawnRightCenter.z - player->GetAttackWorldTransform().translation_.z) * (spawnRightCenter.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[RightCenter];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[RightCenter] = false;
				spawnCount--;
			}
		}

		if (isSpawn[RightBottom] == true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnRightBottom.x - player->GetAttackWorldTransform().translation_.x) * (spawnRightBottom.x - player->GetAttackWorldTransform().translation_.x) + (spawnRightBottom.z - player->GetAttackWorldTransform().translation_.z) * (spawnRightBottom.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[RightBottom];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[RightBottom] = false;
				spawnCount--;
			}
		}

		//各間の当たり判定
		if (isSpawn[LMTop] == true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnLMTop.x - player->GetAttackWorldTransform().translation_.x) * (spawnLMTop.x - player->GetAttackWorldTransform().translation_.x) + (spawnLMTop.z - player->GetAttackWorldTransform().translation_.z) * (spawnLMTop.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[LMTop];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[LMTop] = false;
				spawnCount--;
			}
		}
		if (isSpawn[LMBottom] == true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnLMBottom.x - player->GetAttackWorldTransform().translation_.x) * (spawnLMBottom.x - player->GetAttackWorldTransform().translation_.x) + (spawnLMBottom.z - player->GetAttackWorldTransform().translation_.z) * (spawnLMBottom.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[LMBottom];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[LMBottom] = false;
				spawnCount--;
			}
		}
		if (isSpawn[RMTop] == true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnRMTop.x - player->GetAttackWorldTransform().translation_.x) * (spawnRMTop.x - player->GetAttackWorldTransform().translation_.x) + (spawnRMTop.z - player->GetAttackWorldTransform().translation_.z) * (spawnRMTop.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[RMTop];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[RMTop] = false;
				spawnCount--;
			}
		}
		if (isSpawn[RMBottom] == true)
		{
			if ((3.0f + player->GetBombCharge()) * (3.0f + player->GetBombCharge()) >= (spawnRMBottom.x - player->GetAttackWorldTransform().translation_.x) * (spawnRMBottom.x - player->GetAttackWorldTransform().translation_.x) + (spawnRMBottom.z - player->GetAttackWorldTransform().translation_.z) * (spawnRMBottom.z - player->GetAttackWorldTransform().translation_.z))
			{
				effectWorldTransform = spawnWorldTransform[RMBottom];
				for (int i = 0; i < 10; i++) {
					std::unique_ptr<Effect> newobj = std::make_unique<Effect>();
					newobj->Initialize(player, effectWorldTransform, boxModel, purpleTexture_, 1);
					effects_.push_back(std::move(newobj));
				}

				score->SetTimer(score->GetTimer() + 3);
				audioManager->PlayWave(spawnerBreakSE);
				isSpawn[RMBottom] = false;
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
			if (isSpawn[LeftTop] == false)
			{
				isSpawn[LeftTop] = true;
				if (nowTime < 30)
				{
					spawnTimer = 600;
				}
				else if (nowTime >= 30 && nowTime < 60)
				{
					spawnTimer = 300;
				}
				else if(nowTime >= 60)
				{
					spawnTimer = 180;
				}
				spawnCount++;
				spawnNum = 0;
				startSpawn = false;
			}
			else
			{
				continue;
			}
		}

		if (spawnNum == 2)
		{
			if (isSpawn[MiddleTop] == false)
			{
				isSpawn[MiddleTop] = true;
				if (nowTime < 30)
				{
					spawnTimer = 600;
				}
				else if (nowTime >= 30 && nowTime < 60)
				{
					spawnTimer = 300;
				}
				else if (nowTime >= 60)
				{
					spawnTimer = 180;
				}
				spawnCount++;
				spawnNum = 0;
				startSpawn = false;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 3)
		{
			if (isSpawn[RightTop] == false)
			{
				isSpawn[RightTop] = true;
				if (nowTime < 30)
				{
					spawnTimer = 600;
				}
				else if (nowTime >= 30 && nowTime < 60)
				{
					spawnTimer = 300;
				}
				else if (nowTime >= 60)
				{
					spawnTimer = 180;
				}
				spawnCount++;
				spawnNum = 0;
				startSpawn = false;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 4)
		{
			if (isSpawn[LeftCenter]== false)
			{
				isSpawn[LeftCenter] = true;
				if (nowTime < 30)
				{
					spawnTimer = 600;
				}
				else if (nowTime >= 30 && nowTime < 60)
				{
					spawnTimer = 300;
				}
				else if (nowTime >= 60)
				{
					spawnTimer = 180;
				}
				spawnCount++;
				spawnNum = 0;
				startSpawn = false;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 5)
		{
			if (isSpawn[MiddleCenter] == false)
			{
				isSpawn[MiddleCenter] = true;
				if (nowTime < 30)
				{
					spawnTimer = 600;
				}
				else if (nowTime >= 30 && nowTime < 60)
				{
					spawnTimer = 300;
				}
				else if (nowTime >= 60)
				{
					spawnTimer = 180;
				}
				spawnCount++;
				spawnNum = 0;
				startSpawn = false;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 6)
		{
			if (isSpawn[RightCenter] == false)
			{
				isSpawn[RightCenter] = true;
				if (nowTime < 30)
				{
					spawnTimer = 600;
				}
				else if (nowTime >= 30 && nowTime < 60)
				{
					spawnTimer = 300;
				}
				else if (nowTime >= 60)
				{
					spawnTimer = 180;
				}
				spawnCount++;
				spawnNum = 0;
				startSpawn = false;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 7)
		{
			if (isSpawn[LeftBottom] == false)
			{
				isSpawn[LeftBottom] = true;
				if (nowTime < 30)
				{
					spawnTimer = 600;
				}
				else if (nowTime >= 30 && nowTime < 60)
				{
					spawnTimer = 300;
				}
				else if (nowTime >= 60)
				{
					spawnTimer = 180;
				}
				spawnCount++;
				spawnNum = 0;
				startSpawn = false;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 8)
		{
			if (isSpawn[MiddleBottom] == false)
			{
				if (startSpawn == false)
				{
					isSpawn[MiddleBottom] = true;
					if (nowTime < 30)
					{
						spawnTimer = 600;
					}
					else if (nowTime >= 30 && nowTime < 60)
					{
						spawnTimer = 300;
					}
					else if (nowTime >= 60)
					{
						spawnTimer = 180;
					}
					spawnCount++;
					spawnNum = 0;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 9)
		{

			if (isSpawn[RightBottom] == false)
			{
				isSpawn[RightBottom] = true;
				if (nowTime < 30)
				{
					spawnTimer = 600;
				}
				else if (nowTime >= 30 && nowTime < 60)
				{
					spawnTimer = 300;
				}
				else if (nowTime >= 60)
				{
					spawnTimer = 180;
				}
				spawnCount++;
				spawnNum = 0;
				startSpawn = false;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 10)
		{
			if (isSpawn[LMTop]== false)
			{
				isSpawn[LMTop] = true;
				if (nowTime < 30)
				{
					spawnTimer = 600;
				}
				else if (nowTime >= 30 && nowTime < 60)
				{
					spawnTimer = 300;
				}
				else if (nowTime >= 60)
				{
					spawnTimer = 180;
				}
				spawnCount++;
				spawnNum = 0;
				startSpawn = false;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 11)
		{
			if (isSpawn[LMBottom]== false)
			{
				isSpawn[LMBottom] = true;
				if (nowTime < 30)
				{
					spawnTimer = 600;
				}
				else if (nowTime >= 30 && nowTime < 60)
				{
					spawnTimer = 300;
				}
				else if (nowTime >= 60)
				{
					spawnTimer = 180;
				}
				spawnCount++;
				spawnNum = 0;
				startSpawn = false;
			}
			else
			{
				continue;
			}
		}
		if (spawnNum == 12)
		{
			if (isSpawn[RMTop] == false)
			{
				isSpawn[RMTop] = true;
				if (nowTime < 30)
				{
					spawnTimer = 600;
				}
				else if (nowTime >= 30 && nowTime < 60)
				{
					spawnTimer = 300;
				}
				else if (nowTime >= 60)
				{
					spawnTimer = 180;
				}
				spawnCount++;
				spawnNum = 0;
				startSpawn = false;
			}
			else
			{
				continue;
			}

		}
		if (spawnNum == 13)
		{
			if (isSpawn[RMBottom] == false)
			{
				isSpawn[RMBottom] = true;
				if (nowTime < 30)
				{
					spawnTimer = 600;
				}
				else if (nowTime >= 30 && nowTime < 60)
				{
					spawnTimer = 300;
				}
				else if (nowTime >= 60)
				{
					spawnTimer = 180;
				}
				spawnCount++;
				spawnNum = 0;
				startSpawn = false;
			}
			else
			{
				continue;
			}
		}
	}

#pragma endregion

#pragma region 各スポーン地点の復活
	if (isSpawn[LeftTop] == true)
	{
		EnemySpawn(spawnLeftTop);
	}

	if (isSpawn[MiddleTop] == true)
	{
		EnemyStraightsSpawn(spawnMiddleTop, enemyStraightAngle);
	}

	if (isSpawn[RightTop] == true)
	{
		EnemyStraightsSpawn(spawnRightTop, enemyStraightAngle);
	}

	if (isSpawn[LeftCenter] == true)
	{
		EnemySpawn(spawnLeftCenter);
	}

	if (isSpawn[MiddleCenter] == true)
	{
		EnemyStraightsSpawn(spawnMiddleCenter, enemyStraightAngle);
	}

	if (isSpawn[RightCenter] == true)
	{
		EnemyStraightsSpawn(spawnRightCenter, enemyStraightAngle);
	}

	if (isSpawn[LeftBottom] == true)
	{
		EnemySpawn(spawnLeftBottom);
	}

	if (isSpawn[MiddleBottom] == true)
	{
		EnemyStraightsSpawn(spawnMiddleBottom, enemyStraightAngle);
	}

	if (isSpawn[RightBottom] == true)
	{
		EnemyStraightsSpawn(spawnRightBottom, enemyStraightAngle);
	}

	if (isSpawn[LMTop] == true)
	{
		EnemyStraightsSpawn(spawnLMTop, enemyStraightAngle);
	}
	if (isSpawn[LMBottom] == true)
	{
		EnemyStraightsSpawn(spawnLMBottom, enemyStraightAngle);
	}
	if (isSpawn[RMTop] == true)
	{
		EnemyStraightsSpawn(spawnRMTop, enemyStraightAngle);
	}
	if (isSpawn[RMBottom] == true)
	{
		EnemyStraightsSpawn(spawnRMBottom, enemyStraightAngle);
	}

#pragma endregion 
}