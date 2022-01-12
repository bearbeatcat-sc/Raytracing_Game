#pragma once
#include <memory>
#include <string>
#include <vector>

#include <Game_Object/Actor.h>

class Timer;

class GameTimer
	:public Actor
{
public:
	GameTimer(int endTime);
	~GameTimer() = default;

	bool IsEnd();
	void Reset();

	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:	

	struct Time
	{
		Actor* _object = nullptr;
		int _num = -1;
	};

	void UpdateTimeObject();

private:
	std::shared_ptr<Timer> _gameTimer;
	const int _endTime;

	std::vector<Time> _numberObjects;

	std::string _stringTime;

	int _previousTime;
};
