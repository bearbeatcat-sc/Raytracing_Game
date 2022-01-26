#pragma once

#include <Game_Object/Actor.h>

#include "Player.h"

class Player;
class GameManager;

class PlayerTimeline
	:public Actor
{
public:

	struct PlayerTask
	{
	public:
		PlayerTask(float generateTime,Player::PlayerState changeState)
			:_generateTime(generateTime),_changeState(changeState)
		{
			
		}

		bool IsActive(float currentTime) const
		{
			return currentTime >= _generateTime;
		}

		Player::PlayerState _changeState;
		float _generateTime;

	};

	PlayerTimeline(GameManager* pGameManager);
	~PlayerTimeline() = default;
	void AddTask(float generateTime, Player::PlayerState playerState);
	void Reset();

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	std::shared_ptr<Timer> _generateTimer;
	std::vector<std::shared_ptr<PlayerTask>> _playerTasks;
	Player* _pPlayer;
	GameManager* _pGameManager;
};
