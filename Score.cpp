#include "Score.h"
#include <stdio.h>
#include "TextureManager.h"
void Score::Initialize()
{
	debugText_ = DebugText::GetInstance();

	scoreBonus = 1.0f;
	timeLimit = 60;

	//スコア用画像の読み込み
	numZero = TextureManager::Load("texture/num0Tex.png");
	numOne = TextureManager::Load("texture/num1Tex.png");
	numTwo = TextureManager::Load("texture/num2Tex.png");
	numThree = TextureManager::Load("texture/num3Tex.png");
	numFour = TextureManager::Load("texture/num4Tex.png");
	numFive = TextureManager::Load("texture/num5Tex.png");
	numSix = TextureManager::Load("texture/num6Tex.png");
	numSeven = TextureManager::Load("texture/num7Tex.png");
	numEight = TextureManager::Load("texture/num8Tex.png");
	numNine = TextureManager::Load("texture/num9Tex.png");
}

void Score::Update()
{
	BonusTime--;
	TimerUpdate();
	scoreDigits[0] = score % 10;
	scoreDigits[1] = score / 10 % 10;
	scoreDigits[2] = score / 100 % 10;
	scoreDigits[3] = score / 1000 % 10;
	scoreDigits[4] = score / 10000 % 10;
	scoreDigits[5] = score / 100000;
}

void Score::ScoreAdd()
{
	if (BonusTime > 0)
	{
		scoreBonus += 0.1f;//スコア倍率上昇
	}
	else
	{
		scoreBonus = 1.0f;//スコアボーナスを1.0倍に
	}
	score += 100 * scoreBonus;//スコア増加
	BonusTime = 60;//倍率が上がる時間


}

void Score::SpanerScoreAdd()
{
	if (BonusTime > 0)
	{
		scoreBonus += 0.1f;//スコア倍率上昇
	}
	else
	{
		scoreBonus = 1.0f;//スコアボーナスを1.0倍に
	}
	score += 500 * scoreBonus;//スコア増加
	BonusTime = 60;//倍率が上がる時間
}

void Score::Draw()
{
	TimerDraw();
	ScoreDraw();
}

void Score::TimerDraw()
{
	timerColor = { 1.0f,1.0f,1.0f,1 };
	if (spawnerBreakFlag == true)
	{
		timerColor = { 0.0f,0.9f,0.15f,1 };
		timeAddTimer++;
	}
	if (timeAddTimer > 15)
	{
		spawnerBreakFlag = false;
		timeAddTimer = 0;
	}

	if (damageFlag == true)
	{
		timerColor = { 0.9f,0,0.15f,1 };
		timeSubTimer++;
	}
	if (timeSubTimer > 15)
	{
		damageFlag = false;
		timeSubTimer = 0;
	}

	if (timeLimit <= 10)
	{
		timerColor = { 0.9f,0,0.15f,1 };
	}

#pragma region 1桁目
	if (timerDigits[0] == 0)
	{
		timerDigOne_.reset(Sprite::Create(numZero, Vector2(timerPosX2, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (timerDigits[0] == 1)
	{
		timerDigOne_.reset(Sprite::Create(numOne, Vector2(timerPosX2, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[0] == 2)
	{
		timerDigOne_.reset(Sprite::Create(numTwo, Vector2(timerPosX2, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[0] == 3)
	{
		timerDigOne_.reset(Sprite::Create(numThree, Vector2(timerPosX2, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[0] == 4)
	{
		timerDigOne_.reset(Sprite::Create(numFour, Vector2(timerPosX2, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[0] == 5)
	{
		timerDigOne_.reset(Sprite::Create(numFive, Vector2(timerPosX2, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[0] == 6)
	{
		timerDigOne_.reset(Sprite::Create(numSix, Vector2(timerPosX2, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[0] == 7)
	{
		timerDigOne_.reset(Sprite::Create(numSeven, Vector2(timerPosX2, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[0] == 8)
	{
		timerDigOne_.reset(Sprite::Create(numEight, Vector2(timerPosX2, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (timerDigits[0] == 9)
	{
		timerDigOne_.reset(Sprite::Create(numNine, Vector2(timerPosX2, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));

	}
#pragma endregion

#pragma region 2桁目
	//2桁目
	if (timerDigits[1] == 0)
	{
		timerDigTwo_.reset(Sprite::Create(numZero, Vector2(timerPosX1, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (timerDigits[1] == 1)
	{
		timerDigTwo_.reset(Sprite::Create(numOne, Vector2(timerPosX1, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[1] == 2)
	{
		timerDigTwo_.reset(Sprite::Create(numTwo, Vector2(timerPosX1, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[1] == 3)
	{
		timerDigTwo_.reset(Sprite::Create(numThree, Vector2(timerPosX1, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[1] == 4)
	{
		timerDigTwo_.reset(Sprite::Create(numFour, Vector2(timerPosX1, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[1] == 5)
	{
		timerDigTwo_.reset(Sprite::Create(numFive, Vector2(timerPosX1, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[1] == 6)
	{
		timerDigTwo_.reset(Sprite::Create(numSix, Vector2(timerPosX1, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[1] == 7)
	{
		timerDigTwo_.reset(Sprite::Create(numSeven, Vector2(timerPosX1, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (timerDigits[1] == 8)
	{
		timerDigTwo_.reset(Sprite::Create(numEight, Vector2(timerPosX1, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (timerDigits[1] == 9)
	{
		timerDigTwo_.reset(Sprite::Create(numNine, Vector2(timerPosX1, timerPosY), timerColor,
			Vector2(0.5f, 0.5f)));

	}
#pragma endregion
	timerDigOne_->Draw();
	timerDigTwo_->Draw();

}

void Score::ScoreDraw()
{
	scoreColor = { 1.0f,1.0f,1.0f,1 };
	if (killFlag == true || spawnerBreakFlag == true)
	{
		scoreColor = { 0.0f,0.9f,0.15f,1 };
		scoreAddTimer++;
	}
	if (scoreAddTimer > 15)
	{
		killFlag = false;
		spawnerBreakFlag = false;
		scoreAddTimer = 0;
	}


#pragma region 1桁目
	if (scoreDigits[0] == 0)
	{
		scoreDigOne_.reset(Sprite::Create(numZero, Vector2(scorePosX6, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (scoreDigits[0] == 1)
	{
		scoreDigOne_.reset(Sprite::Create(numOne, Vector2(scorePosX6, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[0] == 2)
	{
		scoreDigOne_.reset(Sprite::Create(numTwo, Vector2(scorePosX6, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[0] == 3)
	{
		scoreDigOne_.reset(Sprite::Create(numThree, Vector2(scorePosX6, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[0] == 4)
	{
		scoreDigOne_.reset(Sprite::Create(numFour, Vector2(scorePosX6, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[0] == 5)
	{
		scoreDigOne_.reset(Sprite::Create(numFive, Vector2(scorePosX6, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[0] == 6)
	{
		scoreDigOne_.reset(Sprite::Create(numSix, Vector2(scorePosX6, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[0] == 7)
	{
		scoreDigOne_.reset(Sprite::Create(numSeven, Vector2(scorePosX6, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[0] == 8)
	{
		scoreDigOne_.reset(Sprite::Create(numEight, Vector2(scorePosX6, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (scoreDigits[0] == 9)
	{
		scoreDigOne_.reset(Sprite::Create(numNine, Vector2(scorePosX6, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
#pragma endregion

#pragma region 2桁目
	//2桁目
	if (scoreDigits[1] == 0)
	{
		scoreDigTwo_.reset(Sprite::Create(numZero, Vector2(scorePosX5, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (scoreDigits[1] == 1)
	{
		scoreDigTwo_.reset(Sprite::Create(numOne, Vector2(scorePosX5, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[1] == 2)
	{
		scoreDigTwo_.reset(Sprite::Create(numTwo, Vector2(scorePosX5, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[1] == 3)
	{
		scoreDigTwo_.reset(Sprite::Create(numThree, Vector2(scorePosX5, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[1] == 4)
	{
		scoreDigTwo_.reset(Sprite::Create(numFour, Vector2(scorePosX5, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[1] == 5)
	{
		scoreDigTwo_.reset(Sprite::Create(numFive, Vector2(scorePosX5, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[1] == 6)
	{
		scoreDigTwo_.reset(Sprite::Create(numSix, Vector2(scorePosX5, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[1] == 7)
	{
		scoreDigTwo_.reset(Sprite::Create(numSeven, Vector2(scorePosX5, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[1] == 8)
	{
		scoreDigTwo_.reset(Sprite::Create(numEight, Vector2(scorePosX5, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (scoreDigits[1] == 9)
	{
		scoreDigTwo_.reset(Sprite::Create(numNine, Vector2(scorePosX5, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
#pragma endregion 

#pragma region 3桁目
	//3桁目
	if (scoreDigits[2] == 0)
	{
		scoreDigThree_.reset(Sprite::Create(numZero, Vector2(scorePosX4, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (scoreDigits[2] == 1)
	{
		scoreDigThree_.reset(Sprite::Create(numOne, Vector2(scorePosX4, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[2] == 2)
	{
		scoreDigThree_.reset(Sprite::Create(numTwo, Vector2(scorePosX4, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[2] == 3)
	{
		scoreDigThree_.reset(Sprite::Create(numThree, Vector2(scorePosX4, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[2] == 4)
	{
		scoreDigThree_.reset(Sprite::Create(numFour, Vector2(scorePosX4, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[2] == 5)
	{
		scoreDigThree_.reset(Sprite::Create(numFive, Vector2(scorePosX4, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[2] == 6)
	{
		scoreDigThree_.reset(Sprite::Create(numSix, Vector2(scorePosX4, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[2] == 7)
	{
		scoreDigThree_.reset(Sprite::Create(numSeven, Vector2(scorePosX4, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[2] == 8)
	{
		scoreDigThree_.reset(Sprite::Create(numEight, Vector2(scorePosX4, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (scoreDigits[2] == 9)
	{
		scoreDigThree_.reset(Sprite::Create(numNine, Vector2(scorePosX4, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
#pragma endregion 

#pragma region 4桁目
	//4桁目
	if (scoreDigits[3] == 0)
	{
		scoreDigFour_.reset(Sprite::Create(numZero, Vector2(scorePosX3, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (scoreDigits[3] == 1)
	{
		scoreDigFour_.reset(Sprite::Create(numOne, Vector2(scorePosX3, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[3] == 2)
	{
		scoreDigFour_.reset(Sprite::Create(numTwo, Vector2(scorePosX3, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[3] == 3)
	{
		scoreDigFour_.reset(Sprite::Create(numThree, Vector2(scorePosX3, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[3] == 4)
	{
		scoreDigFour_.reset(Sprite::Create(numFour, Vector2(scorePosX3, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[3] == 5)
	{
		scoreDigFour_.reset(Sprite::Create(numFive, Vector2(scorePosX3, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[3] == 6)
	{
		scoreDigFour_.reset(Sprite::Create(numSix, Vector2(scorePosX3, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[3] == 7)
	{
		scoreDigFour_.reset(Sprite::Create(numSeven, Vector2(scorePosX3, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[3] == 8)
	{
		scoreDigFour_.reset(Sprite::Create(numEight, Vector2(scorePosX3, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (scoreDigits[3] == 9)
	{
		scoreDigFour_.reset(Sprite::Create(numNine, Vector2(scorePosX3, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
#pragma endregion 

#pragma region 5桁目
	//5桁目
	if (scoreDigits[4] == 0)
	{
		scoreDigFive_.reset(Sprite::Create(numZero, Vector2(scorePosX2, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (scoreDigits[4] == 1)
	{
		scoreDigFive_.reset(Sprite::Create(numOne, Vector2(scorePosX2, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[4] == 2)
	{
		scoreDigFive_.reset(Sprite::Create(numTwo, Vector2(scorePosX2, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[4] == 3)
	{
		scoreDigFive_.reset(Sprite::Create(numThree, Vector2(scorePosX2, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[4] == 4)
	{
		scoreDigFive_.reset(Sprite::Create(numFour, Vector2(scorePosX2, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[4] == 5)
	{
		scoreDigFive_.reset(Sprite::Create(numFive, Vector2(scorePosX2, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[4] == 6)
	{
		scoreDigFive_.reset(Sprite::Create(numSix, Vector2(scorePosX2, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[4] == 7)
	{
		scoreDigFive_.reset(Sprite::Create(numSeven, Vector2(scorePosX2, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[4] == 8)
	{
		scoreDigFive_.reset(Sprite::Create(numEight, Vector2(scorePosX2, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (scoreDigits[4] == 9)
	{
		scoreDigFive_.reset(Sprite::Create(numNine, Vector2(scorePosX2, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
#pragma endregion 

#pragma region 6桁目
	//6桁目
	if (scoreDigits[5] == 0)
	{
		scoreDigSix_.reset(Sprite::Create(numZero, Vector2(scorePosX1, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (scoreDigits[5] == 1)
	{
		scoreDigSix_.reset(Sprite::Create(numOne, Vector2(scorePosX1, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[5] == 2)
	{
		scoreDigSix_.reset(Sprite::Create(numTwo, Vector2(scorePosX1, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[5] == 3)
	{
		scoreDigSix_.reset(Sprite::Create(numThree, Vector2(scorePosX1, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[5] == 4)
	{
		scoreDigSix_.reset(Sprite::Create(numFour, Vector2(scorePosX1, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[5] == 5)
	{
		scoreDigSix_.reset(Sprite::Create(numFive, Vector2(scorePosX1, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[5] == 6)
	{
		scoreDigSix_.reset(Sprite::Create(numSix, Vector2(scorePosX1, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[5] == 7)
	{
		scoreDigSix_.reset(Sprite::Create(numSeven, Vector2(scorePosX1, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));
	}
	else if (scoreDigits[5] == 8)
	{
		scoreDigSix_.reset(Sprite::Create(numEight, Vector2(scorePosX1, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
	else if (scoreDigits[5] == 9)
	{
		scoreDigSix_.reset(Sprite::Create(numNine, Vector2(scorePosX1, scorePosY), scoreColor,
			Vector2(0.5f, 0.5f)));

	}
#pragma endregion 
	scoreDigOne_->Draw();
	scoreDigTwo_->Draw();
	scoreDigThree_->Draw();
	scoreDigFour_->Draw();
	scoreDigFive_->Draw();
	scoreDigSix_->Draw();
}

void Score::Reset()
{
	score = 0;
	BonusTime = 0;
	scoreBonus = 1.0f;
	timeLimit = 60;
	timerFlame = 60;
	isFinish = false;
}

bool Score::IsFinish()
{
	return isFinish;
}

void Score::TimerUpdate()
{
	timerFlame--;
	if (timerFlame <= 0)
	{
		timeLimit--;
		timerFlame = 60;
	}
	timerDigits[0] = timeLimit % 10;
	timerDigits[1] = timeLimit / 10;

	if (timeLimit <= 0)
	{
		isFinish = true;
	}
	//
	if (timeLimit > 99)
	{
		timeLimit = 99;
	}
}

int Score::GetScore()
{
	return score;
}

void Score::SetSpawnerBreakFlag(bool flag)
{
	spawnerBreakFlag = flag;
}

void Score::SetDamageFlag(bool flag)
{
	damageFlag = flag;
}

void Score::SetKillFlag(bool flag)
{
	killFlag = flag;
}

void Score::SetTimeAddTimer()
{
	timeAddTimer = 0;
}

void Score::SetTimeSubTimer()
{
	timeAddTimer = 0;
}

void Score::SetScoreAddTimer()
{
	scoreAddTimer = 0;
}

Score* Score::GetInstance()
{
	static Score instance;
	return &instance;
}