#pragma once
#include "DebugText.h"
#include <memory>
#include "Sprite.h"
class Score
{
private:
	DebugText* debugText_ = nullptr;

	int score = 0;
	int BonusTime = 0;
	float scoreBonus = 1.0f;
	int timeLimit = 60;
	int timerFlame = 60;
	bool isFinish = false;

	//スコア用画像(0~9のため10枚分)
	uint32_t numZero = 0;
	uint32_t numOne = 0;
	uint32_t numTwo = 0;
	uint32_t numThree = 0;
	uint32_t numFour = 0;
	uint32_t numFive = 0;
	uint32_t numSix = 0;
	uint32_t numSeven = 0;
	uint32_t numEight = 0;
	uint32_t numNine = 0;

	//スコア用スプライト
	std::unique_ptr<Sprite> scoreDigOne_;
	std::unique_ptr<Sprite> scoreDigTwo_;
	std::unique_ptr<Sprite> scoreDigThree_;
	std::unique_ptr<Sprite> scoreDigFour_;
	std::unique_ptr<Sprite> scoreDigFive_;
	std::unique_ptr<Sprite> scoreDigSix_;

	//制限時間用スプライト
	std::unique_ptr<Sprite> timerDigOne_;
	std::unique_ptr<Sprite> timerDigTwo_;

	//スコアの最大桁数
	int scoreDigits[10] = {};

	//制限時間用の桁数
	int timerDigits[10] = {};

	float timerPosX1 = 610;
	float timerPosX2 = 670;
	float timerPosY = 70;
	Vector4 timerColor = { 1.0f,1.0f,1.0f,1.0f };

	float scorePosX1 = 880;
	float scorePosX2 = 940;
	float scorePosX3 = 1000;
	float scorePosX4 = 1060;
	float scorePosX5 = 1120;
	float scorePosX6 = 1180;
	float scorePosY = 70;
	Vector4 scoreColor = { 1.0f,1.0f,1.0f,1.0f };

	bool damageFlag = false;
	bool spawnerBreakFlag = false;
	bool killFlag = false;

	int timeAddTimer = 0;
	int timeSubTimer = 0;
	int scoreAddTimer = 0;

public:
	void Initialize();
	void Update();
	void ScoreAdd();
	void SpanerScoreAdd();

	void Draw();

	void TimerDraw();
	void ScoreDraw();

	void Reset();
	bool IsFinish();

	void TimerUpdate();

	int GetScore();
	void SetSpawnerBreakFlag(bool flag);
	void SetDamageFlag(bool flag);
	void SetKillFlag(bool flag);
	void SetTimeAddTimer();
	void SetTimeSubTimer();
	void SetScoreAddTimer();

	void SetTimer(int time) { timeLimit = time; }
	int GetTimer() { return timeLimit; }

	static Score* GetInstance();
};