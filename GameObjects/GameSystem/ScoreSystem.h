#pragma once
#include <Game_Object/Actor.h>

class Player;


class ScoreSystem
	:public Actor
{
public:
	ScoreSystem();
	~ScoreSystem() = default;
	void AddScore(int score);

private:

	struct Score
	{
		Actor* _object = nullptr;
		int _num = -1;
	};

	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

	void UpdateScoreObject();

private:
	std::vector<Score> _numberObjects;

	std::string _stringScore;
	int _score;
};
