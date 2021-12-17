#pragma once

#include <Game_Object/Actor.h>

class ScoreSystem;
class Player;

class GameManager
	:public Actor
{
public:
	GameManager() = default;
	~GameManager() = default;
	void AddScore(const int score);
	Player* GetPlayer();

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

	void StartGame();

private:
	Player* _pPlayer;
	ScoreSystem* _pScoreSystem;
};
