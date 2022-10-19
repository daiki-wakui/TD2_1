#pragma once
#include "DebugText.h"

class Score
{
private:
	DebugText* debugText_ = nullptr;

	int score = 0;
	int BonusTime = 0;
	float scoreBonus = 1.0f;
	int timeLimit = 65;
	int minutes = 3;
	int seconds = 0;

public:
	void Initialize();
	void Update();
	void ScoreAdd();
	void Draw();

	static Score* GetInstance();
};