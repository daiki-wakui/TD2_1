#include "Score.h"
#include <stdio.h>

void Score::Initialize()
{
	debugText_ = DebugText::GetInstance();

	scoreBonus = 1.0f;
	timeLimit = 60;
	minutes = 0;
	seconds = 30;
}

void Score::Update()
{
	BonusTime--;
	timeLimit--;

	if (timeLimit <= 0) {
		seconds--;

		if (seconds < 0 && isFnish == false) {
			minutes--;
			seconds = 59;
		}
		
		timeLimit = 60;
	}

	if (minutes <= 0) {
		minutes = 0;
	}

	if (minutes == 0 && seconds <= 0) {
		seconds = 0;
		isFnish = true;
	}
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
	BonusTime = 50;//倍率が上がる時間
}

void Score::Draw()
{
	debugText_->SetPos(1000, 60);
	debugText_->Printf("time %d", timeLimit);

	debugText_->SetPos(1000, 80);
	debugText_->Printf("minutes,second %2d:%2d", minutes, seconds);
	if (seconds < 10) {
		debugText_->SetPos(1162, 80);
		debugText_->Printf("0");
	}
}

void Score::Reset()
{
	score = 0;
	BonusTime = 0;
	scoreBonus = 1.0f;
	timeLimit = 60;
	minutes = 3;
	seconds = 0;
	isFnish = false;
}

bool Score::IsFnish()
{
	return isFnish;
}

Score* Score::GetInstance()
{
	static Score instance;
	return &instance;
}