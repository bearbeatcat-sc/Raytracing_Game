#pragma once
#include <Game_Object/Actor.h>

class Player;


class ScoreSystem
	:public Actor
{
public:
	ScoreSystem(const SimpleMath::Vector3& generatePos);
	~ScoreSystem() = default;

	void AddScore(int score);
	void ChangeResultMode();

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

	bool _isEnd;

	SimpleMath::Vector3 _resultPosition;
	SimpleMath::Vector3 _generatePosition;

	std::shared_ptr<Timer> _pMoveResultTimer;
};
